import base64
import concurrent.futures
import time
from concurrent.futures.thread import ThreadPoolExecutor
from pathlib import Path

import requests

URL = "http://localhost:8000/api/execute_script"

BASE_DIR = Path(__file__).parent
TEST_FILE_PATH = BASE_DIR / "graph_data" / "Cube.graphml"
LUA_SCRIPT_PATH = BASE_DIR / "lua_script" / "hierarchical.lua"


def send_request(thread_id, lua_script, encoded_graph):
    payload = {
        "type": "LuaScript",
        "script": lua_script,
        "input_data": [{
            "id": f"test_graph_{thread_id}",
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

        if response.status_code == 200:
            output_data = response.json().get("output_data", [])
            return thread_id, "success" if output_data else "success_no_data", duration, None
        else:
            return thread_id, "failure", duration, response.text

    except Exception as e:
        return thread_id, "exception", 0, str(e)


def run_concurrency_test(num_threads=20):
    if not TEST_FILE_PATH.exists():
        print(f"[ERROR] Graph file not found: {TEST_FILE_PATH}")
        return

    if not LUA_SCRIPT_PATH.exists():
        print(f"[ERROR] Lua script not found: {LUA_SCRIPT_PATH}")
        return

    print(f"Reading graph file '{TEST_FILE_PATH}'...")
    encoded_graph = base64.b64encode(TEST_FILE_PATH.read_bytes()).decode("utf-8")
    lua_script = LUA_SCRIPT_PATH.read_text(encoding="utf-8")

    print(f"Starting concurrency test with {num_threads} threads...")

    success_count = 0

    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        futures = [executor.submit(send_request, i, lua_script, encoded_graph) for i in range(num_threads)]

        print("\n-------------- THREAD EXECUTION LOG --------------")
        print(f"{'Thread ID':<12} | {'Status':<10} | {'Total Round-Trip (Client -> Server -> Client)'}")
        print("-" * 50)

        for future in concurrent.futures.as_completed(futures):
            tid, status, duration, msg = future.result()

            tid_str = f"Thread-{tid:02d}"
            if status == "success":
                print(f"{tid_str:<12} | {'SUCCESS':<10} | {duration:.4f} seconds")
                success_count += 1
            elif status == "success_no_data":
                print(f"{tid_str:<12} | {'SUCCESS (No Data)':<10} | {duration:.4f} seconds")
                success_count += 1
            else:
                print(f"{tid_str:<12} | {'FAILURE':<10} | {msg}")

        print("-" * 50)
        print("\n------------ CONCURRENCY TEST RESULTS ------------")
        print(f"Total Threads:           {num_threads}")
        print(f"Successful Executions:   {success_count}")
        print(f"Failed Executions:       {num_threads - success_count}")

        rate = (success_count / num_threads * 100) if num_threads > 0 else 0
        print(f"Success Rate:            {rate:.1f}%")
        print(f"-" * 50)


if __name__ == "__main__":
    run_concurrency_test(30)
