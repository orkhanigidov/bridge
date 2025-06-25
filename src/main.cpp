#include "core/Engine.hpp"
#include "pipeline/PipelineManager.hpp"
#include "registration/LuaRegistrar.hpp"
#include "serialization/PipelineSerializer.hpp"

std::atomic_bool running{true};
void signalHandler(int signal)
{
    std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

int main(int argc, char* argv[])
{
    try
    {
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        std::string host = "localhost";
        int port         = 8080;

        if (argc > 1)
        {
            host = argv[1];
        }
        if (argc > 2)
        {
            port = std::stoi(argv[2]);
        }

        std::cout << "Starting engine service..." << std::endl;

        engine::registration::LuaRegistrar::instance().register_all();

        std::string json_str = R"({
          "pipelines": [
            {
              "pipeline": "graph_pipeline",
              "steps": [
                {"new": "Graph", "as": "G"},
                {"new": "GraphAttributes", "as": "GA", "with": ["$G"]},
                {"call": "read", "with": ["$GA", "$G", "unix-history.gml"]},
                {"new": "SugiyamaLayout", "as": "SL"},
                {"new": "OptimalHierarchyLayout", "as": "OHL"},
                {"call": "$OHL.layerDistance", "with": [30.0]},
                {"call": "$OHL.nodeDistance", "with": [25.0]},
                {"call": "$OHL.weightBalancing", "with": [0.8]},
                {"call": "$SL.setLayout", "with": ["$OHL"]},
                {"call": "$SL.call", "with": ["$GA"]},
                {"call": "write", "with": ["$GA", "output-unix-history-hierarchical.gml"]},
                {"call": "write", "with": ["$GA", "output-unix-history-hierarchical.svg"]}
              ]
            }
          ]
        })";

        auto request = engine::serialization::PipelineSerializer::from_json(json_str);

        engine::pipeline::PipelineManager pipeline_manager;
        for (const auto& pipeline : *request->pipelines)
        {
            pipeline_manager.load(*pipeline->name, *pipeline.get());
        }

        for (const auto& pipeline : *request->pipelines)
        {
            pipeline_manager.execute(*pipeline->name);
        }

        // auto& engine = engine::core::Engine::instance();
        // engine.initialize(host, port);
        // engine.start();
        //
        // std::cout << "Engine service running on http:// " << host << ":" << port << std::endl;
        // std::cout << "Press Ctrl+C to exit" << std::endl;
        //
        // while (running)
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        //
        // std::cout << "Shutting down engine service..." << std::endl;
        // engine.shutdown();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}