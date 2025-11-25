import base64

import requests

import test_utils

GRAPH_FILE = "Cube.graphml"
CSV_FILENAME = "security_audit.csv"


def run_security_test():
    test_utils.print_banner("SECURITY SANDBOX AUDIT")

    if not test_utils.check_server_health():
        test_utils.log_error("Server is not running! Cannot start security test.")
        return

    graph_path, _ = test_utils.check_files(GRAPH_FILE)
    if not graph_path: return

    try:
        encoded_graph = base64.b64encode(graph_path.read_bytes()).decode("utf-8")
    except IOError as e:
        test_utils.log_error(f"File reading error: {e}")
        return

    attacks = [
        {
            "name": "OS Command Injection",
            "type": "Isolation",
            "script": '''
            local ok, err = pcall(function()
                os.execute("whoami")
            end)
            if not ok then error("SANDBOX_BLOCKED: os.execute") end
            if os then error("FAIL: os table exists!") end
            ''',
            "expected_keywords": ["SANDBOX_BLOCKED", "attempt to index", "nil value"]
        },
        {
            "name": "File System Write",
            "type": "Isolation",
            "script": '''
            local ok, err = pcall(function()
                io.open("/tmp/hacked.txt", "w")
            end)
            if not ok then error("SANDBOX_BLOCKED: io.open") end
            if io then error("FAIL: io table exists!") end
            ''',
            "expected_keywords": ["SANDBOX_BLOCKED", "attempt to index", "nil value"]
        },
        {
            "name": "Concatenation Bypass",
            "type": "Evasion",
            "script": '''
            local ok, err = pcall(function()
                local target = _G["o".."s"]
                target.execute("whoami")
            end)
            if not ok then error("SANDBOX_BLOCKED: Dynamic Access") end
            ''',
            "expected_keywords": ["SANDBOX_BLOCKED", "attempt to index", "nil value"]
        },
        {
            "name": "Dynamic Code Loading",
            "type": "RCE Prevention",
            "script": '''
            local ok, err = pcall(function()
                local func = load("return os.execute('whoami')")()
            end)
            if not ok then error("SANDBOX_BLOCKED: Dynamic Load") end
            ''',
            "expected_keywords": ["SANDBOX_BLOCKED", "attempt to index", "nil value"]
        },
        {
            "name": "Infinite Loop (DoS)",
            "type": "Availability",
            "script": 'while true do end',
            "expected_keywords": ["timeout", "time limit"]
        }
    ]

    results_data = []
    headers = ["Attack Name", "Type", "Outcome", "Server Status", "Details"]
    widths = [25, 15, 10, 15, 40]
    test_utils.log_table_header(headers, widths)

    for attack in attacks:
        payload = {
            "type": "LuaScript",
            "script": attack['script'],
            "input_data": [{"id": "sec_test", "chunk_index": 0, "total_chunks": 1, "chunk_data": encoded_graph}],
            "options": {"output_data_format": "graphml", "timeout_milliseconds": 2000}
        }

        try:
            response = requests.post(test_utils.EXECUTE_URL, json=payload, timeout=5)

            if response.status_code != 200:
                err_body = response.json().get('error', {})
                server_msg = err_body.get('message', response.text)

                if any(k in server_msg for k in attack['expected_keywords']):
                    outcome = "PASS"
                else:
                    outcome = "WARNING"
            else:
                outcome = "FAIL"
                server_msg = "Code Executed Successfully!"

        except requests.exceptions.ReadTimeout:
            if "DoS" in attack['name']:
                outcome = "PASS"
                server_msg = "Request Timed Out (Correct)"
            else:
                outcome = "ERROR"
                server_msg = "Unexpected Network Timeout"

        except Exception as e:
            outcome = "ERROR"
            server_msg = str(e)

        is_alive = test_utils.check_server_health()
        server_status = "ONLINE" if is_alive else "CRASHED"

        if not is_alive: outcome = "CRITICAL"

        clean_msg = server_msg.replace('\n', ' ')[:37] + '...' if len(server_msg) > 37 else server_msg.replace('\n', ' ')
        test_utils.log_table_row([attack['name'], attack['type'], outcome, server_status, clean_msg], widths)

        results_data.append({
            "attack_name": attack['name'],
            "attack_type": attack['type'],
            "outcome": outcome,
            "server_status": server_status,
            "details": server_msg
        })

    test_utils.save_to_csv(CSV_FILENAME, list(results_data[0].keys()), results_data)


if __name__ == "__main__":
    run_security_test()
