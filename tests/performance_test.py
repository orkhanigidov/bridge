import base64
import random
import time

import numpy as np
import requests

import test_utils

CSV_FILENAME = "performance_benchmark.csv"

ITERATIONS_PER_SCENARIO = 100
WARMUP_REQUESTS = 10

SCENARIOS = [
    {"name": "CPU Stress", "graph": "Mesh.graphml", "script": "orthogonal.lua"},
    {"name": "Algorithmic Complexity", "graph": "n100e100.graphml", "script": "hierarchical.lua"},
    {"name": "Data Volume Stress", "graph": "n1000e1000.graphml", "script": "energybased.lua"}
]


def ensure_graph_exists(nodes, edges):
    filename = f"n{nodes}e{edges}.graphml"
    path = test_utils.GRAPH_DATA_DIR / filename

    if path.exists(): return path

    test_utils.log_info(f"Generating {filename} ({nodes} nodes, {edges} edges)...")
    test_utils.GRAPH_DATA_DIR.mkdir(parents=True, exist_ok=True)

    with path.open("w", encoding="utf-8") as f:
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n'
                '<graphml xmlns="http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">\n'
                '  <graph id="G" edgedefault="directed">\n')

        for i in range(nodes):
            f.write(f'    <node id="n{i}"/>\n')

        for _ in range(edges):
            source, target = random.randint(0, nodes - 1), random.randint(0, nodes - 1)
            if source != target:
                f.write(f'    <edge source="n{source}" target="n{target}"/>\n')

        f.write('  </graph>\n</graphml>\n')
    return path


def run_performance_test():
    test_utils.print_banner("FULL SYSTEM PERFORMANCE BENCHMARK")

    if not test_utils.check_server_health():
        test_utils.log_error("Server is not running! Cannot start performance test.")
        return

    results_data = []

    for scenario in SCENARIOS:
        test_utils.log_info(f"Starting Scenario: {scenario['name']} ({scenario['graph']} + {scenario['script']})")

        # graph_path = ensure_graph_exists(scenario['nodes'], scenario['edges'])
        graph_path, script_path = test_utils.check_files(scenario['graph'], scenario['script'])
        if not graph_path or not script_path: continue

        try:
            encoded_graph = base64.b64encode(graph_path.read_bytes()).decode('utf-8')
            script_content = script_path.read_text(encoding='utf-8')
        except IOError as e:
            test_utils.log_error(f"File reading error: {e}")
            continue

        file_size_mb = graph_path.stat().st_size / (1024 * 1024)  # Convert to MB

        payload = {
            "type": "LuaScript",
            "script": script_content,
            "input_data": [{"id": "perf_test", "chunk_index": 0, "total_chunks": 1, "chunk_data": encoded_graph}],
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

        baseline_mem = test_utils.get_server_memory()
        peak_mem = baseline_mem

        latencies_total, latencies_script, latencies_overhead = [], [], []

        print(f"    -> Running {ITERATIONS_PER_SCENARIO} iterations...", end='', flush=True)
        with requests.Session() as session:
            for _ in range(ITERATIONS_PER_SCENARIO):
                try:
                    start_time = time.perf_counter()
                    response = session.post(test_utils.EXECUTE_URL, json=payload)
                    end_time = time.perf_counter()

                    current_mem = test_utils.get_server_memory()
                    if current_mem and current_mem > peak_mem: peak_mem = current_mem

                    if response.status_code == 200:
                        total_ms = (end_time - start_time) * 1000  # Convert to ms
                        script_ms = float(response.json().get('metadata', {}).get('duration_milliseconds', 0))

                        latencies_total.append(total_ms)
                        latencies_script.append(script_ms)
                        latencies_overhead.append(total_ms - script_ms)

                except Exception:
                    pass
            print(" Done.")

        if latencies_total:
            stats = {
                "scenario": scenario['name'],
                "graph": scenario['graph'],
                "file_size_mb": round(file_size_mb, 2),
                "total_time_mean_ms": round(np.mean(latencies_total), 2),
                "script_time_mean_ms": round(np.mean(latencies_script), 2),
                "overhead_mean_ms": round(np.mean(latencies_overhead), 2),
                "total_time_p95_ms": round(np.percentile(latencies_total, 95), 2),
                "total_time_p99_ms": round(np.percentile(latencies_total, 99), 2),
                "total_time_std_dev": round(np.std(latencies_total), 2),
                "baseline_mem_mb": round(baseline_mem, 2),
                "peak_mem_mb": round(peak_mem, 2),
                "mem_growth_mb": round(peak_mem - baseline_mem, 2),
            }

            results_data.append(stats)

            print("-" * 50)
            print("TIMING & MEMORY METRICS SUMMARY")
            print(f"  -> File Size:       {stats['file_size_mb']} MB")
            print(f"  -> Mean Latency:    {stats['total_time_mean_ms']} ms (Script: {stats['script_time_mean_ms']} ms)")
            print(f"  -> P95 / P99:       {stats['total_time_p95_ms']} ms / {stats['total_time_p99_ms']} ms")
            print(f"  -> Std Dev:         {stats['total_time_std_dev']} ms")
            print(f"  -> Overhead:        {stats['overhead_mean_ms']} ms ({(stats['overhead_mean_ms'] / stats['total_time_mean_ms']) * 100:.1f}%)")
            print(f"  -> Baseline Memory: {stats['baseline_mem_mb']} MB")
            print(f"  -> Peak Memory:     {stats['peak_mem_mb']} MB (Growth: {stats['mem_growth_mb']} MB)")
            print("-" * 50)

    test_utils.save_to_csv(CSV_FILENAME, list(results_data[0].keys()), results_data)


if __name__ == "__main__":
    run_performance_test()
