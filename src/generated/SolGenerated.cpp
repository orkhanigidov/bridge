// Auto-generated file. Do not edit manually!

#include "ogdf/basic/GraphAttributes.h"
#include "ogdf/basic/graph_generators/randomized.h"
#include "ogdf/fileformats/GraphIO.h"
#include "ogdf/layered/OptimalHierarchyLayout.h"
#include "ogdf/layered/SugiyamaLayout.h"
#include "registration/FunctionRegistrar.hpp"
#include "registration/LuaRegistrar.hpp"

#include <ogdf/basic/Graph.h>
#include <sol/sol.hpp>

using namespace ogdf;

namespace engine::registration
{
    void register_classes(sol::state& lua)
    {
        lua.new_usertype<Graph>("Graph", sol::constructors<Graph>());

        lua.new_usertype<GraphAttributes>("GraphAttributes",
                                          sol::constructors<GraphAttributes(const Graph&)>());

        lua.new_usertype<SugiyamaLayout>(
            "SugiyamaLayout", sol::constructors<SugiyamaLayout()>(), "setLayout",
            sol::resolve<void(HierarchyLayoutModule * pLayout)>(&SugiyamaLayout::setLayout), "call",
            sol::resolve<void(GraphAttributes & AG)>(&SugiyamaLayout::call),
            sol::meta_function::garbage_collect, [] {});

        lua.new_usertype<HierarchyLayoutModule>("HierarchyLayoutModule", sol::no_constructor);
        lua.new_usertype<OptimalHierarchyLayout>(
            "OptimalHierarchyLayout", sol::constructors<OptimalHierarchyLayout()>(),
            sol::base_classes, sol::bases<HierarchyLayoutModule>(), "layerDistance",
            sol::resolve<void(double x)>(&OptimalHierarchyLayout::layerDistance), "nodeDistance",
            sol::resolve<void(double x)>(&OptimalHierarchyLayout::nodeDistance), "weightBalancing",
            sol::resolve<void(double w)>(&OptimalHierarchyLayout::weightBalancing),
            sol::meta_function::garbage_collect, [] {});
    }

    void register_global_functions(sol::state& lua)
    {
        lua.set_function("read", [](GraphAttributes& GA, Graph& G, const string& filename) -> bool
                         { return GraphIO::read(GA, G, filename); });

        lua.set_function("write", [](const GraphAttributes& GA, const string& filename) -> bool
                         { return GraphIO::write(GA, filename); });
    }
} // namespace engine::registration
