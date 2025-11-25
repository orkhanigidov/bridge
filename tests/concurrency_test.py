import base64
import concurrent.futures
import time
from concurrent.futures.thread import ThreadPoolExecutor

import requests

import test_utils

GRAPH_FILE = "n100e100.graphml"
SCRIPT_FILE = "hierarchical.lua"
CSV_FILENAME = "concurrency_scalability.csv"

THREAD_LEVELS = [1, 10, 20, 50, 100]
REQUESTS_PER_THREAD = 10


def send_single_request(session, payload):
    start = time.perf_counter()
    try:
        response = session.post(test_utils.EXECUTE_URL, json=payload, timeout=30)
        duration = time.perf_counter() - start

        if response.status_code == 200:
            has_output = bool(response.json().get('output_data'))
            status = "SUCCESS" if has_output else "SUCCESS_NO_DATA"
            return duration, status, None
        else:
            return duration, "HTTP_ERROR", response.text

    except Exception as e:
        return (time.perf_counter() - start), "EXCEPTION", str(e)


def run_concurrency_test():
    test_utils.print_banner("CONCURRENCY & SCALABILITY ANALYSIS")

    if not test_utils.check_server_health():
        test_utils.log_error("Server is not running! Cannot start concurrency test.")
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
        "input_data": [{"id": "conc_test", "chunk_index": 0, "total_chunks": 1, "chunk_data": encoded_graph}],
        "options": {"output_data_format": "graphml"}
    }

    results_data = []
    headers = ["Threads", "Total Req", "Throughput (Req/s)", "Avg Latency (s)", "Errors", "Data Integrity"]
    widths = [10, 12, 20, 18, 10, 15]
    test_utils.log_table_header(headers, widths)

    for num_threads in THREAD_LEVELS:
        total_requests = num_threads * REQUESTS_PER_THREAD

        start_global = time.perf_counter()

        latencies = []
        status_counts = {"SUCCESS": 0, "SUCCESS_NO_DATA": 0, "HTTP_ERROR": 0, "EXCEPTION": 0}

        with ThreadPoolExecutor(max_workers=num_threads) as executor:
            futures = []

            for _ in range(num_threads):
                session = requests.Session()
                for _ in range(REQUESTS_PER_THREAD):
                    futures.append(executor.submit(send_single_request, session, payload))

            for future in concurrent.futures.as_completed(futures):
                duration, status, msg = future.result()
                latencies.append(duration)
                status_counts[status] += 1

        total_time = time.perf_counter() - start_global

        valid_success = status_counts['SUCCESS'] + status_counts['SUCCESS_NO_DATA']
        throughput = valid_success / total_time if total_time > 0 else 0
        avg_latency = sum(latencies) / len(latencies) if latencies else 0
        total_errors = status_counts['HTTP_ERROR'] + status_counts['EXCEPTION']

        integrity_status = "VERIFIED"
        if total_errors > 0:
            integrity_status = f"FAILED ({total_errors})"
        elif status_counts['SUCCESS_NO_DATA'] > 0:
            integrity_status = "PARTIALLY (Empty Output)"

        test_utils.log_table_row([
            num_threads, total_requests, f"{throughput:.2f}", f"{avg_latency:.4f}", total_errors, integrity_status
        ], widths)

        results_data.append({
            "threads": num_threads,
            "total_requests": total_requests,
            "throughput": round(throughput, 2),
            "avg_latency": round(avg_latency, 4),
            "error_rate": round((total_errors / total_requests) * 100, 2),
            "success_count": status_counts['SUCCESS'],
            "empty_response_count": status_counts['SUCCESS_NO_DATA'],
            "integrity_status": integrity_status
        })

    test_utils.save_to_csv(CSV_FILENAME, list(results_data[0].keys()), results_data)


if __name__ == "__main__":
    run_concurrency_test()
