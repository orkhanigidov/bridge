import base64
import random
import sys
import threading
import time
from pathlib import Path

import psutil
import requests

NODES = 10000
EDGES = 10000

URL = "http://localhost:8000/api/execute_script"

BASE_DIR = Path(__file__).parent
TEMP_DIR = BASE_DIR / "temp"
TEST_FILE_PATH = TEMP_DIR / f"n{NODES}e{EDGES}.graphml"
LUA_SCRIPT_PATH = BASE_DIR / "lua_script" / "hierarchical.lua"
SERVER_PROCESS_NAME = "Engine"


def get_server_process():
    for proc in psutil.process_iter(['pid', 'name', 'memory_info']):
        try:
            if SERVER_PROCESS_NAME.lower() in proc.info['name'].lower():
                return proc
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            continue
    return None


def generate_graph(path: Path, nodes, edges):
    print(f"Generating graph: {nodes} nodes, {edges} edges...")
    path.parent.mkdir(exist_ok=True)

    with path.open("w", encoding="utf-8") as f:
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        f.write('<graphml xmlns="http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">\n')
        f.write('  <graph id="G" edgedefault="directed">\n')

        for i in range(nodes):
            f.write(f'    <node id="n{i}"/>\n')

        for _ in range(edges):
            source, target = random.randint(0, nodes - 1), random.randint(0, nodes - 1)
            if source != target:
                f.write(f'    <edge source="n{source}" target="n{target}"/>\n')

        f.write('  </graph>\n')
        f.write('</graphml>\n')
    print(f"Graph generated at: {path}")


def monitor_resources(pid, stop_event, stats):
    try:
        process = psutil.Process(pid)
        process.cpu_percent(interval=None)
        while not stop_event.is_set():
            mem = process.memory_info().rss / 1024 / 1024  # MB
            if mem > stats["peak_memory"]:
                stats["peak_memory"] = mem

            cpu = process.cpu_percent(interval=0.1)
            if cpu > stats["peak_cpu"]:
                stats["peak_cpu"] = cpu

    except Exception as e:
        print(f"[ERROR] Resource monitoring failed: {e}")


def run_performance_test():
    generate_graph(TEST_FILE_PATH, NODES, EDGES)

    file_size_mb = TEST_FILE_PATH.stat().st_size / 1024 / 1024
    print(f"File size: {file_size_mb:.2f} MB")

    if not LUA_SCRIPT_PATH.exists():
        print(f"[ERROR] Lua script not found: {LUA_SCRIPT_PATH}")
        return

    print(f"Reading graph file '{TEST_FILE_PATH}'...")
    encoded_graph = base64.b64encode(TEST_FILE_PATH.read_bytes()).decode("utf-8")
    lua_script = LUA_SCRIPT_PATH.read_text(encoding="utf-8")

    process = get_server_process()
    if not process:
        print(f"[ERROR] Server process '{SERVER_PROCESS_NAME}' not found. Please ensure the server is running.")
        sys.exit(1)

    print("Starting performance test...")

    stats = {
        "peak_memory": process.memory_info().rss / 1024 / 1024,
        "peak_cpu": 0.0
    }
    initial_mem = stats["peak_memory"]
    stop_event = threading.Event()

    res_thread = threading.Thread(target=monitor_resources, args=(process.pid, stop_event, stats))
    res_thread.start()

    payload = {
        "type": "LuaScript",
        "script": lua_script,
        "input_data": [{
            "id": "perf_test",
            "chunk_index": 0,
            "total_chunks": 1,
            "chunk_data": encoded_graph
        }],
        "options": {
            "output_data_format": "graphml"
        }
    }

    try:
        start_time = time.time()
        response = requests.post(URL, json=payload)
        duration = time.time() - start_time
    except Exception as e:
        print(f"[ERROR] Request failed: {e}")
        stop_event.set()
        res_thread.join()
        return

    stop_event.set()
    res_thread.join()

    if response.status_code == 200:
        script_exec_time = response.json().get("metadata", {}).get("duration_milliseconds", 0) / 1000.0
        system_overhead = duration - script_exec_time
        output_len = sum(len(c["chunk_data"]) for c in response.json().get("output_data", []))
        output_size_mb = output_len / 1024 / 1024

        print("\n--------------------- RESULTS --------------------")
        print("1. TIMING METRICS")
        print(f"  Total Round-Trip:     {duration:.4f} seconds (Client -> Server -> Client)")
        print(f"  Script Execution:     {script_exec_time:.4f} seconds")
        print(f"  System Overhead:      {system_overhead:.4f} seconds (I/O, Base64, Network)")
        print(f"-" * 50)
        print("2. DATA METRICS")
        print(f"  Input File Size:      {file_size_mb:.2f} MB")
        print(f"  Output Data Size:     {output_size_mb:.2f} MB")
        print(f"-" * 50)
        print("3. RESOURCE USAGE (Server)")
        print(f"  RAM (Initial):        {initial_mem:.2f} MB")
        print(f"  RAM (Peak):           {stats['peak_memory']:.2f} MB")
        print(f"  RAM (Increase):       {stats['peak_memory'] - initial_mem:+.2f} MB")

        cpu_count = psutil.cpu_count(logical=True)
        cpu_peak = stats["peak_cpu"]
        per_core = cpu_peak / cpu_count
        print(f"  CPU (Peak Usage):     {cpu_peak:.1f}% (total), {per_core:.1f}% per core on {cpu_count} logical CPUs")
        print(f"-" * 50)
    else:
        print(f"[ERROR] Request failed with status code: {response.status_code} - {response.text}")


if __name__ == "__main__":
    run_performance_test()
