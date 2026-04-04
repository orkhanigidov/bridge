# BRIDGE (Binding Runtime Interface for Dynamic General Execution)

A resilient hybrid architecture and middleware connecting the **Open Graph Drawing Framework (OGDF)** to **Unity (GAV-VR)** via automated Lua bindings for high-performance immersive graph analysis.

## 📖 About

**BRIDGE** is a master thesis project designed to solve the "Technical Debt" and interoperability challenges between managed VR environments (C#/.NET) and high-performance native graph libraries (C++).

Visualizing complex networks in Virtual Reality often leads to "hairball" structures. While C++ libraries like **OGDF** offer powerful layout algorithms to resolve this, integrating them into Unity is notoriously difficult due to memory management conflicts and the manual effort required to wrap extensive APIs.

### Key Features

* **Hybrid Architecture:** Seamlessly bridges C# (Unity) and C++ (OGDF) using **Lua** as a lightweight intermediary layer.
* **Automated Binding Generation ("Codegen"):** Uses **LibClang** AST analysis to automatically generate type-safe C++ binding code for Lua, eliminating manual wrapper maintenance.
* **Dual Execution Strategy:**
    * **Native Mode:** Runs locally via P/Invoke for low-latency VR interactions.
    * **Network Mode:** Runs as a standalone microservice (REST API via **Oat++**) for offloading heavy computations to the cloud.
* **Safety & Security:** Implements a strict whitelist sandbox and thread-local state management to ensure memory safety and concurrency.

### System Architecture

The following diagram illustrates the data flow across the Code Generation Tool, the BRIDGE Engine, and the Client.

```mermaid
flowchart TB
    subgraph IO_Parsers["IO &amp; Configuration"]
        EnvReader["Env Reader"]
        YamlReader["Yaml Reader"]
        HeaderCollector["Header Collector"]
    end
    subgraph Analysis["Static Analysis"]
        ClangAnalyzer["Clang Analyzer (LibClang)"]
        AstVisitor["AST Visitor"]
    end
    subgraph Generation["Code Synthesis"]
        Sol2Generator["Sol2 Generator"]
    end
    subgraph Codegen["Code Generation Tool (Pre-Build)"]
        ConfigFile["Config (.yaml & .env)"]
        InputFiles[("OGDF Header Files & Wrappers")]
        IO_Parsers
        Analysis
        Generation
        GeneratedBindings["Generated Lua Bindings (.cpp)"]
    end
    subgraph Binding_Layer["Binding Layer"]
        LuaBinder["Lua Binder"]
        ManualRegisters["Manual Registers"]
        GeneratedBindings_In["Generated Bindings"]
    end
    subgraph Scripting_Core["Scripting Core"]
        LuaStateManager["Lua State Manager"]
        ScriptExecutor["Script Executor"]
        LuaScriptRunner["Lua Script Runner"]
        OGDF["OGDF Library"]
    end
    subgraph Execution_Logic["Execution Logic"]
        ExecutionEngine["Execution Engine"]
        ExecutionService["Execution Service"]
    end
    subgraph Native_Interface["Native Interface"]
        InteropCAPI["Interop C API"]
        InteropBridge["Interop Bridge"]
        ResponseFactory["Response Factory"]
    end
    subgraph Network_Interface["Network Interface"]
        ServerManager["Server Manager (Oat++)"]
        ExecutionController["Execution Controller"]
        Mapper["Mapper (DTOs)"]
    end
    subgraph Engine["C++ Engine (Shared Library / Server)"]
        Binding_Layer
        Scripting_Core
        Execution_Logic
        Native_Interface
        Network_Interface
    end
    subgraph Infrastructure["Infrastructure"]
        AppServices["App Services"]
        ResourceLoader["Resource Loader"]
    end
    subgraph UI_Control["UI & Control"]
        ScriptListController["Script List Controller"]
        IntegrationController["Integration Controller"]
        ModalController["Modal Controller"]
        ParameterExtractor["Parameter Extractor"]
        ScriptBuilder["Script Builder"]
    end
    subgraph Execution_Strategy["Execution Strategy"]
        ExecutorFactory{"Executor Factory"}
        NativeExecutor["Native Executor"]
        NetworkExecutor["Network Executor"]
        NativeBridge["Native Bridge (P/Invoke)"]
    end
    subgraph Async_Jobs["Async Jobs"]
        GraphUpdateJob["Graph Update Job"]
    end
    subgraph Client["C# GAV-VR Client (Unity)"]
        direction TB
        CSHConfig["Configurations &amp; Scripts"]
        Infrastructure
        UI_Control
        Execution_Strategy
        Async_Jobs
    end
    CSHConfig --> ResourceLoader
    InputFiles --> HeaderCollector
    ConfigFile --> EnvReader & YamlReader
    HeaderCollector --> ClangAnalyzer
    YamlReader --> ClangAnalyzer
    ClangAnalyzer --> AstVisitor
    AstVisitor -- Metadata (Class/Enum/Func ) --> Sol2Generator
Sol2Generator --> GeneratedBindings
GeneratedBindings --> GeneratedBindings_In
GeneratedBindings_In --> LuaBinder
ManualRegisters --> LuaBinder
LuaStateManager -- Uses --> LuaBinder
LuaScriptRunner -- Gets State --> LuaStateManager
LuaScriptRunner --> ScriptExecutor
ScriptExecutor -- Calls --> OGDF
InteropCAPI --> InteropBridge
InteropBridge -- Error Case --> ResponseFactory
ExecutionEngine -- Success Case --> ResponseFactory
InteropBridge --> ExecutionEngine
ExecutionEngine --> LuaScriptRunner
ServerManager --> ExecutionController
ExecutionController --> ExecutionService
ExecutionController -- Uses --> Mapper
ExecutionService -- Base64 Decode/Encode --> LuaScriptRunner
ResourceLoader --> AppServices
AppServices --> ScriptListController & ModalController
ScriptListController -- Selects --> ModalController
ModalController --> IntegrationController
ModalController -- Uses --> ParameterExtractor
IntegrationController -- Uses --> ScriptBuilder
ScriptBuilder --> ExecutorFactory
ExecutorFactory -- Mode: Native --> NativeExecutor
ExecutorFactory -- Mode: Network --> NetworkExecutor
NativeExecutor --> NativeBridge
NativeBridge == FFI Call ==> InteropCAPI
NetworkExecutor == HTTP / JSON ( Base64 ) ==> ServerManager
IntegrationController -- On Success --> GraphUpdateJob

classDef cpp fill: #f9f, stroke: #333, stroke-width: 2px
classDef csharp fill: #9cf, stroke: #333, stroke-width: 2px
classDef tool fill: #ff9, stroke: #333, stroke-width: 2px
```

## 🚀 Installation

### Prerequisites

* **C++ Compiler:** GCC 13.3.0+ or MSVC (C++17 support required).
* **Build System:** CMake (3.28+), Ninja.
* **Dependencies:** Lua 5.4, OGDF (Elderberry), Oat++, Sol2, LibClang.

### Setup Steps

1.  **Clone the Repository**
    ```bash
    git clone https://github.com/orkhanigidov/BRIDGE.git
    cd BRIDGE
    ```
2.  **Environment Configuration**
    Copy the example environment file and configure your paths (specifically `LIBRARY_INCLUDE_PATH` for OGDF headers).
    ```bash
    cp .env.example .env
    ```
3.  **Build the Engine**
    Use the provided bootstrap scripts to install dependencies and compile.
    ```powershell
    # Windows (PowerShell)
    # Builds as a Shared Library (DLL) for Unity Local Execution
    ./bootstrap.ps1 -BuildType Release -InstallDeps --build-shared
    ```
    ```bash
    # Linux (Bash)
    # Builds as a Standalone Server for Cloud Execution
    chmod +x bootstrap.sh
    sudo ./bootstrap.sh --build-type Release --install-deps
    ```

## 💻 Usage

The BRIDGE Engine can operate in two modes depending on your compilation target.

### 1. Configuration (Bindings)

Before building, define which OGDF classes and methods to expose to Lua in `bindings_config.yaml`:

```yaml
classes:
  - name: Graph
    methods:
      - numberOfNodes
  - name: SugiyamaLayout
free_functions:
  - randomSimpleGraph
```

### 2. Execution Modes

* **A. Native Mode (Unity P/Invoke)**
  If built with `--build-shared`, the output is `Engine.dll`.
  * The build script automatically copies `Engine.dll` and `lua54.dll` to your Unity project's `Assets/Plugins/` folder (if `UNITY_PROJECT_PATH` is set in `.env`).
  * In GAV-VR options, select **"Native"**.
 
* **B. Network Mode (Cloud/Server)**
  If built without the shared flag, the output is an executable (`Engine` or `Engine.exe`).
  * Run the server:
  ```bash
  ./build/Engine --host 0.0.0.0 --port 8000
  ```
  * In GAV-VR options, select **"Network"** and configure `NetworkConfiguration.json`:
  ```json
  {
      "protocol": "http",
      "host": "localhost",
      "port": 8000,
      "endpoint": "/api/execute_script"
  }
  ```

### 3. Scripting Example

Write Lua scripts to control C++ algorithms dynamically. Files are placed in the `Scripts/` directory.

**Example: Hierarchical Layout (`hierarchical.lua`)**

```lua
-- 'GA' (GraphAttributes) and 'G' (Graph) are injected by the Engine
read(GA, G, __input__) -- __input__ is auto-replaced by the Engine

sl = SugiyamaLayout()
sl:setRanking(OptimalRanking())
sl:setCrossMin(MedianHeuristic())

ohl = OptimalHierarchyLayout()
ohl:layerDistance(30.0)
ohl:nodeDistance(25.0)

sl:setLayout(ohl)
sl:call(GA) -- Executes C++ OGDF code

write(GA, __output__) -- __output__ is handled by the Engine
```

## 📊 Evaluation & Benchmarks

The BRIDGE architecture was tested in a containerized environment to validate its performance, scalability, memory stability, and security. Based on the master thesis experimental results, the system demonstrates efficiency for immersive VR analytics:

* **Performance & Minimal Overhead:** The hybrid architecture introduces negligible latency for heavy graph processing workloads. In data volume stress tests (1,000 nodes, ~1.21 MB payload), the system overhead (serialization, transport, and Lua VM initialization) accounted for only **1.3%** of the total execution time.
* **Concurrency & Scalability:** The `thread_local` state management successfully isolates the Lua Virtual Machine, preventing race conditions without global locks. Under I/O bound stress tests with up to 100 concurrent threads, the system maintained a **0.0% error rate** with stable throughput (peaking at 64 Req/s).
* **Memory Stability:** Stress tests with over 2,000 sequential requests confirmed the robustness of the memory handshake protocol. The system showed no linear memory leaks; memory growth safely plateaued (+2.23 MB for fragmentation stress) due to Lua's incremental garbage collection and efficient buffer caching.
* **Security Sandboxing:** The "Whitelist by Default" policy successfully neutralized simulated malicious scripts during audit. The execution environment safely blocked OS command injection, unauthorized file system access, dynamic code loading, and evasion attempts, while infinite loops were safely terminated by execution timeouts.
* **Visual Fidelity in VR:** Algorithmic calculation precision was flawless when compared to native C++ 2D vector outputs. While the backend successfully transmits all spatial data and layout coordinates, minor visual discrepancies (such as missing orthogonal bends or orientation artifacts) were identified as isolated limitations within the frontend GAV-VR GraphML parser.

> **⚠️ Note:** These tests were conducted in a synthetic local containerized environment (Docker) and driven by a single client machine. They do not account for real-world network variables (such as WAN latency, packet loss, or bandwidth fluctuations) or distributed multi-client loads. Therefore, while they confirm baseline stability within defined parameters, these benchmark results should not be considered fully reliable or representative of real-world production environments.

## 📄 License

This project is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**. See the [LICENSE](https://github.com/orkhanigidov/BRIDGE/blob/with_sol2/LICENSE) file for details.

* **Copyright:** © 2025 Orkhan Igidov
* **Thesis Context:** Connecting OGDF to GAV-VR - Design Concept and Realisation (Universität Konstanz).
