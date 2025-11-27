import csv
from pathlib import Path

import docker
import requests

SERVER_HOST = "localhost"
SERVER_PORT = 8000
BASE_URL = f"http://{SERVER_HOST}:{SERVER_PORT}"
EXECUTE_URL = f"{BASE_URL}/api/execute_script"
HEALTH_URL = f"{BASE_URL}/api/health"

SERVER_PROCESS_NAME = "Engine"
CONTAINER_NAME = "engine-server"

BASE_DIR = Path(__file__).parent
OUTPUT_DIR = BASE_DIR / "results"
GRAPH_DATA_DIR = BASE_DIR / "graph_data"
LUA_SCRIPT_DIR = BASE_DIR / "lua_script"

OUTPUT_DIR.mkdir(exist_ok=True)


def print_banner(text):
    print("\n" + "=" * 80)
    print(f" {text.center(78)}")
    print("=" * 80)


def log_info(msg):
    print(f"[INFO]    {msg}")


def log_success(msg):
    print(f"[SUCCESS] {msg}")


def log_warning(msg):
    print(f"[WARNING] {msg}")


def log_error(msg):
    print(f"[ERROR]   {msg}")


def log_table_header(headers, widths):
    header_str = " | ".join(f"{h:<{w}}" for h, w in zip(headers, widths))
    print("-" * len(header_str))
    print(header_str)
    print("-" * len(header_str))


def log_table_row(values, widths):
    clean_values = [str(v) for v in values]
    row_str = " | ".join(f"{str(v):<{w}}" for v, w in zip(clean_values, widths))
    print(row_str)


try:
    docker_client = docker.from_env()
except Exception:
    docker_client = None
    log_error("Docker client could not be initialized.")


def get_server_memory():
    if not docker_client:
        return 0.0

    try:
        container = docker_client.containers.get(CONTAINER_NAME)
        stats = container.stats(stream=False)
        return stats["memory_stats"]["usage"] / (1024 * 1024)  # Convert to MB
    except Exception as e:
        log_error(f"Error retrieving memory usage: {e}")
        return 0.0


# def get_server_memory():
#     for proc in psutil.process_iter(['pid', 'name', 'memory_info']):
#         try:
#             if SERVER_PROCESS_NAME.lower() in proc.info['name'].lower():
#                 return proc.info['memory_info'].rss / 1024 / 1024  # Convert to MB
#         except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
#             continue
#     return None


def check_server_health():
    try:
        response = requests.get(HEALTH_URL, timeout=2)
        return response.status_code == 200
    except requests.RequestException:
        return False


def check_files(graph_file, script_file=None):
    graph_path = GRAPH_DATA_DIR / graph_file
    if not graph_path.exists():
        log_error(f"Graph file not found: {graph_path}")
        return None, None

    script_path = None
    if script_file:
        script_path = LUA_SCRIPT_DIR / script_file
        if not script_path.exists():
            log_error(f"Lua script not found: {script_path}")
            return None, None

    return graph_path, script_path


def save_to_csv(filename, fieldnames, data_list):
    filepath = OUTPUT_DIR / filename
    file_exists = filepath.exists()
    try:
        with open(filepath, mode='a' if file_exists else 'w', newline='', encoding='utf-8') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            if not file_exists:
                writer.writeheader()
            writer.writerows(data_list)
        log_success(f"Data saved to: {filepath.name}")
    except IOError as e:
        log_error(f"Failed to save CSV: {e}")
