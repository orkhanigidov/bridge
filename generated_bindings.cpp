// Auto-generated file. Do not edit manually!

#include "engine/bindings/lua/class_registrar.hpp"
#include "engine/bindings/lua/free_function_registrar.hpp"
#include "ogdf/basic/GraphAttributes.h"
#include "ogdf/fileformats/GraphIO.h"
#include "ogdf/layered/MedianHeuristic.h"
#include "ogdf/layered/OptimalHierarchyLayout.h"
#include "ogdf/layered/OptimalRanking.h"
#include "ogdf/layered/SugiyamaLayout.h"

#include <sol/sol.hpp>

using namespace ogdf;

namespace engine::bindings::lua
{
    void register_classes(sol::state& lua)
    {
        ClassRegistrar<Graph>(lua, "Graph").constructor<>();

        ClassRegistrar<GraphAttributes>(lua, "GraphAttributes")
            .constructor<const Graph&, long>()
            .variable("nodeGraphics", GraphAttributes::nodeGraphics)
            .variable("edgeGraphics", GraphAttributes::edgeGraphics)
            .variable("threeD", GraphAttributes::threeD);

        ClassRegistrar<OptimalRanking>(lua, "OptimalRanking").constructor<>().base_classes<RankingModule>();
        ClassRegistrar<MedianHeuristic>(lua, "MedianHeuristic").constructor<>().base_classes<LayeredCrossMinModule>();

        ClassRegistrar<SugiyamaLayout>(lua, "SugiyamaLayout")
            .constructor<>()
            .enable_gc()
            .method("setRanking", sol::resolve<void(RankingModule * pRanking)>(&SugiyamaLayout::setRanking))
            .method("setCrossMin", sol::resolve<void(LayeredCrossMinModule * pCrossMin)>(&SugiyamaLayout::setCrossMin))
            .method("setLayout", sol::resolve<void(HierarchyLayoutModule * pLayout)>(&SugiyamaLayout::setLayout))
            .method("call", sol::resolve<void(GraphAttributes & AG)>(&SugiyamaLayout::call));

        ClassRegistrar<OptimalHierarchyLayout>(lua, "OptimalHierarchyLayout")
            .constructor<>()
            .base_classes<HierarchyLayoutModule>()
            .enable_gc()
            .method("layerDistance", sol::resolve<void(double x)>(&OptimalHierarchyLayout::layerDistance))
            .method("nodeDistance", sol::resolve<void(double x)>(&OptimalHierarchyLayout::nodeDistance))
            .method("weightBalancing", sol::resolve<void(double w)>(&OptimalHierarchyLayout::weightBalancing));

        ClassRegistrar<HierarchyLayoutModule>(lua, "HierarchyLayoutModule");
    }

    void register_free_functions(sol::state& lua)
    {
        FreeFunctionRegistrar(lua)
            .function("read", [](GraphAttributes& GA, Graph& G, const string& filename) -> bool { return GraphIO::read(GA, G, filename); })
            .function("write", [](const GraphAttributes& GA, const string& filename) -> bool { return GraphIO::write(GA, "Assets/Resources/Graphs/" + filename); });
    }
} // namespace engine::lua_bindings