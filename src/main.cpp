#include "core/Engine.hpp"
#include "generated/ReflectionRegistration.cpp"
#include "network/NetworkManager.hpp"
#include "pch.hpp"
#include "pipeline/PipelineManager.hpp"
#include "reflection/MethodRegistrar.hpp"
#include "reflection/ReflectionRegistry.hpp"
#include "serialization/RttrConverter.hpp"
#include "sol/sol.hpp"


nlohmann::json processMessage(const std::string& message)
{
    try
    {
        nlohmann::json request = nlohmann::json::parse(message);

        if (request.contains("pipelines") && request["pipelines"].is_array())
        {
            nlohmann::json results = nlohmann::json::array();
            for (const auto& pipeline : request["pipelines"])
            {
                try
                {
                    engine::pipeline::PipelineManager manager;
                    std::string name = pipeline["pipeline"].get<std::string>();
                    manager.load_pipeline(name, pipeline);
                    manager.execute(name);

                    return nlohmann::json{{"status", "success"},
                                          {"message", "Pipeline executed successfully"},
                                          {"pipeline", name}};
                }
                catch (const std::exception& e)
                {
                    return nlohmann::json{
                        {"status", "error"},
                        {"message", std::string("Pipeline execution failed: ") + e.what()},
                        {"pipeline", request.value("pipeline", "unknown")}};
                }
            }
        }

        if (!request.contains("method") || !request["method"].is_string())
        {
            throw std::invalid_argument("Missing or invalid 'method' field");
        }

        std::string methodName = request["method"];
        if (methodName == "getRegisteredMethods")
        {
            return engine::core::Engine::getInstance().getRegisteredMethods();
        }

        nlohmann::json params = request.value("params", nlohmann::json::object());

        return engine::core::Engine::getInstance().executeMethod(methodName, params);
    }
    catch (const nlohmann::json::exception& e)
    {
        throw std::invalid_argument("Invalid JSON format: " + std::string(e.what()));
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Error processing message: " + std::string(e.what()));
    }
}

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

        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table);

        register_with_sol2(lua);

        std::string script = R"(
            local graph = ogdf.Graph()
            local ga = ogdf.GraphAttributes(graph)
            ogdf.read(ga, graph, "build/unix-history.gml")

            local sugiyama = ogdf.SugiyamaLayout()
            local optimalHierarchy = ogdf.OptimalHierarchyLayout()
            optimalHierarchy:layerDistance(50)
            optimalHierarchy:nodeDistance(20)
            optimalHierarchy:weightBalancing(0.5)

            sugiyama:setLayout(optimalHierarchy)
            sugiyama:call(ga)

            ogdf.write(ga, "build/output.gml")
            ogdf.write(ga, "build/output.svg")
        )";

        try
        {
            lua.script(script);
        }
        catch (const sol::error& e)
        {
            std::cerr << e.what() << std::endl;
        }


        std::string endpoint = "tcp://*:5555";
        if (argc > 1)
        {
            endpoint = argv[1];
        }

        std::cout << "Starting engine service..." << std::endl;

        engine::reflection::ReflectionRegistry::instance().register_all_from_rttr();

        engine::network::NetworkManager networkManager(processMessage);
        networkManager.initialize(endpoint);
        networkManager.startMessageLoop();

        std::cout << "Engine service running on " << endpoint << std::endl;
        std::cout << "Press Ctrl+C to exit" << std::endl;

        while (running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Shutting down engine service..." << std::endl;
        networkManager.shutdown();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}