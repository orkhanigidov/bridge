import base64
import time

import requests

import test_utils

GRAPH_FILE = "Cube.graphml"
SCRIPT_FILE = "energybased.lua"
CSV_TIMELINE = "memory_stability_timeline.csv"
CSV_SUMMARY = "memory_stability_summary.csv"

TOTAL_REQUESTS = 500
WARMUP_REQUESTS = 50
CHECK_INTERVAL = 20


def run_memory_test():
    test_utils.print_banner("MEMORY OWNERSHIP & STABILITY TEST")

    if not test_utils.check_server_health():
        test_utils.log_error("Server is not running! Cannot start memory test.")
        return

    graph_path, script_path = test_utils.check_files(GRAPH_FILE, SCRIPT_FILE)
    if not graph_path or not script_path: return

    try:
        encoded_graph = base64.b64encode(graph_path.read_bytes()).decode('utf-8')
        script_content = script_path.read_text(encoding='utf-8')
    except IOError as e:
        test_utils.log_error(f"File reading error: {e}")
        return

    payload = {
        "type": "LuaScript",
        "script": script_content,
        "input_data": [{"id": "mem_stress", "chunk_index": 0, "total_chunks": 1, "chunk_data": encoded_graph}],
        "options": {"output_data_format": "graphml"}
    }

    print(f"    -> Warming up ({WARMUP_REQUESTS} requests)...", end='', flush=True)
    with requests.Session() as session:
        for _ in range(WARMUP_REQUESTS):
            try:
                session.post(test_utils.EXECUTE_URL, json=payload)
            except:
                pass
    print(" Done.")

    time.sleep(1)
    baseline_mem = test_utils.get_server_memory()
    if baseline_mem is None:
        test_utils.log_error("Could not read server memory.")
        return

    test_utils.log_info(f"Baseline Memory: {baseline_mem:.2f} MB")

    print(f"    -> Executing Stress Test ({TOTAL_REQUESTS} requests)...")

    memory_records = []
    peak_mem = baseline_mem
    start_time = time.perf_counter()

    headers = ["Request #", "Current Mem (MB)", "Delta (MB)"]
    widths = [15, 20, 15]
    test_utils.log_table_header(headers, widths)

    with requests.Session() as session:
        for i in range(1, TOTAL_REQUESTS + 1):
            try:
                session.post(test_utils.EXECUTE_URL, json=payload)

                if i % CHECK_INTERVAL == 0 or i == TOTAL_REQUESTS:
                    current_mem = test_utils.get_server_memory()
                    if current_mem is None:
                        test_utils.log_error("Server process crashed or stopped.")
                        break

                    if current_mem > peak_mem: peak_mem = current_mem
                    delta = current_mem - baseline_mem

                    test_utils.log_table_row([i, f"{current_mem:.2f}", f"{delta:+.2f}"], widths)

                    memory_records.append(
                        {"request_count": i, "memory_mb": round(current_mem, 2), "delta_mb": round(delta, 2)})

            except Exception as e:
                test_utils.log_error(f"Request {i} failed: {e}")
                break

    duration = time.perf_counter() - start_time
    final_mem = test_utils.get_server_memory()
    net_change = final_mem - baseline_mem if final_mem else 0

    test_utils.save_to_csv(CSV_TIMELINE, list(memory_records[0].keys()), memory_records)

    print("-" * 50)
    print("MEMORY METRICS SUMMARY")
    print(f"  Execution Time:          {duration:.2f} sec")
    print(f"  Baseline Memory:         {baseline_mem:.2f} MB")
    print(f"  Peak Memory:             {peak_mem:.2f} MB")
    print(f"  Final Memory:            {final_mem:.2f} MB")
    print(f"  Net Change:              {net_change:+.2f} MB")
    print("-" * 50)

    if net_change > 20.0:
        test_utils.log_error("Significant Memory Leak Detected (>20 MB)")
    elif net_change > 5.0:
        test_utils.log_warning("Minor Memory Accumulation (Fragmentation?) Detected (>5 MB)")
    else:
        test_utils.log_success("Memory Usage is Stable (Proper RAII/GC behavior)")

    summary_data = [{
        "total_requests": TOTAL_REQUESTS,
        "execution_time_sec": round(duration, 2),
        "baseline_mem_mb": round(baseline_mem, 2),
        "peak_mem_mb": round(peak_mem, 2),
        "final_mem_mb": round(final_mem, 2),
        "net_change_mb": round(net_change, 2),
        "status": "PASS" if net_change < 5.0 else "WARNING" if net_change < 20.0 else "FAIL"
    }]

    test_utils.save_to_csv(CSV_SUMMARY, list(summary_data[0].keys()), summary_data)


if __name__ == "__main__":
    run_memory_test()
