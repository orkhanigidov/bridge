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
            "name": "OS Command Execution Attack",
            "script": '''
            local ok, err = pcall(function()
                os.execute("whoami")
            end)
            if not ok then error("os.execute blocked: ".. tostring(err)) end
            ''',
            "expected_keyword": "blocked"
        },
        {
            "name": "File System Access Attack",
            "script": '''
            local ok, err = pcall(function()
                local f = io.open("testfile.txt", "w")
                if f then
                    f:write("test")
                    f:close()
                else
                    error("io.open blocked")
                end
            end)
            if not ok then error("file access blocked: ".. tostring(err)) end
            ''',
            "expected_keyword": "blocked"
        },
        {
            "name": "Concatenation Bypass Attack",
            "script": '''
            local ok, err = pcall(function()
                local o, s = "o", "s"
                _G[o..s]["execute"]("whoami")
            end)
            if not ok then error("bypass blocked: ".. tostring(err)) end
            ''',
            "expected_keyword": "blocked"
        },
        {
            "name": "Dynamic Code Loading Attack",
            "script": '''
            local ok, err = pcall(function()
                local code = [[io.open("/etc/passwd","r")]]
                load(code)()
            end)
            if not ok then error("dynamic load blocked: ".. tostring(err)) end
            ''',
            "expected_keyword": "blocked"
        },
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
