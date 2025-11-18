import base64
import sys
import time
from pathlib import Path

import psutil
import requests

TOTAL_REQUESTS = 500
CHECK_INTERVAL = 50

URL = "http://localhost:8000/api/execute_script"

BASE_DIR = Path(__file__).parent
TEST_FILE_PATH = BASE_DIR / "graph_data" / "Cube.graphml"
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


def run_memory_test():
    if not TEST_FILE_PATH.exists():
        print(f"[Error] Graph file not found: {TEST_FILE_PATH}")
        return

    if not LUA_SCRIPT_PATH.exists():
        print(f"[Error] Lua script not found: {LUA_SCRIPT_PATH}")
        return

    print(f"Reading graph file '{TEST_FILE_PATH}'...")
    encoded_graph = base64.b64encode(TEST_FILE_PATH.read_bytes()).decode("utf-8")
    lua_script = LUA_SCRIPT_PATH.read_text(encoding="utf-8")

    process = get_server_process()
    if not process:
        print(f"[Error] Server process '{SERVER_PROCESS_NAME}' not found. Please ensure the server is running.")
        sys.exit(1)

    print("Starting memory stress test...")
    print(f"Target Process: {process.name()} (PID: {process.pid})")

    initial_mem = process.memory_info().rss / 1024 / 1024  # MB

    start_time = time.time()

    with requests.Session() as session:
        for i in range(1, TOTAL_REQUESTS + 1):
            payload = {
                "type": "LuaScript",
                "script": lua_script,
                "input_data": [{
                    "id": f"mem_test_{i}",
                    "chunk_index": 0,
                    "total_chunks": 1,
                    "chunk_data": encoded_graph
                }],
                "options": {
                    "output_data_format": "graphml"
                }
            }

            try:
                response = session.post(URL, json=payload)
                if response.status_code != 200:
                    print(f"[ERROR] Request-{i} failed with status code: {response.status_code}")
            except Exception as e:
                print(f"[ERROR] Request-{i} failed: {e}")

            if i % CHECK_INTERVAL == 0:
                try:
                    current_mem = process.memory_info().rss / 1024 / 1024  # MB
                    diff = current_mem - initial_mem
                    print(f"  Req: {i:3}/{TOTAL_REQUESTS} | Mem: {current_mem:6.2f} MB | Diff: {diff:+6.2f} MB")
                except psutil.NoSuchProcess:
                    print("[ERROR] Server process stopped unexpectedly.")
                    return

    end_time = time.time()

    try:
        final_mem = process.memory_info().rss / 1024 / 1024  # MB
    except psutil.NoSuchProcess:
        print("[WARNING] Test finished, but server process could not be found.")
        return

    diff_total = final_mem - initial_mem

    print("\n--------------------- RESULTS --------------------")
    print("1. EXECUTION TIMING")
    print(f"  Total Duration:           {end_time - start_time:.2f} seconds")
    print(f"  Total Requests:           {TOTAL_REQUESTS}")
    print(f"  Average Time/Request:     {(end_time - start_time) / TOTAL_REQUESTS * 1000:.2f} milliseconds")
    print(f"-" * 50)
    print("2. MEMORY METRICS (Server)")
    print(f"  RAM (Initial):            {initial_mem:.2f} MB")
    print(f"  RAM (Final):              {final_mem:.2f} MB")
    print(f"  RAM (Net Change):         {diff_total:+.2f} MB")
    print(f"-" * 50)

    if diff_total > 20:
        print("[WARNING] Significant memory increase detected. Possible memory leak!")
    else:
        print("[SUCCESS] Memory usage is stable.")


if __name__ == "__main__":
    run_memory_test()
