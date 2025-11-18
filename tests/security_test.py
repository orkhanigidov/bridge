import base64
from pathlib import Path

import requests

URL = "http://localhost:8000/api/execute_script"

BASE_DIR = Path(__file__).parent
TEST_FILE_PATH = BASE_DIR / "graph_data" / "Cube.graphml"


def run_security_test():
    if not TEST_FILE_PATH.exists():
        print(f"[ERROR] Graph file not found: {TEST_FILE_PATH}")
        return

    encoded_graph = base64.b64encode(TEST_FILE_PATH.read_bytes()).decode("utf-8")

    attacks = [
        {
            "name": "File System Access Attack",
            "script": 'local file = io.open("hacked.txt", "w"); file:write("Security breach!"); file:close()',
            "expected_keyword": "io"
        },
        {
            "name": "OS Command Execution Attack",
            # 92 is the ASCII code for backslash '\', used in Windows paths
            "script": '''
            local is_windows = package.config:sub(1,1):byte() == 92
            if is_windows then
                os.execute("shutdown /s /t 0")
            else
                os.execute("shutdown -h now")
            end
            ''',
            "expected_keyword": "os"
        }
    ]

    print("Starting security sandbox test...")

    for attack in attacks:
        payload = {
            "type": "LuaScript",
            "script": attack["script"],
            "input_data": [{
                "id": "sec_test",
                "chunk_index": 0,
                "total_chunks": 1,
                "chunk_data": encoded_graph
            }],
            "options": {
                "output_data_format": "graphml"
            }
        }

        try:
            response = requests.post(URL, json=payload)

            status = response.json().get("status", "")
            error_msg = response.json().get("error", {}).get("message", "")

            if status == "Failure" and attack["expected_keyword"] in error_msg:
                print(f"[SUCCESS] {attack['name']} blocked.")
                print(f"    -> Server Response: {error_msg}")
            else:
                print(f"[FAILURE] {attack['name']} was NOT blocked or returned unexpected response.")
                print(f"    -> Server Response: {error_msg}")
        except Exception as e:
            print(f"[ERROR] Exception during {attack['name']} test: {e}")


if __name__ == "__main__":
    run_security_test()
