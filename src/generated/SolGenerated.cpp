// Auto-generated file. Do not edit manually!

#include "ogdf/basic/GraphAttributes.h"
#include "ogdf/fileformats/GraphIO.h"
#include "ogdf/layered/OptimalHierarchyLayout.h"
#include "ogdf/layered/SugiyamaLayout.h"
#include "registration/ClassRegistrar.hpp"
#include "registration/FunctionRegistrar.hpp"
#include "registration/LuaRegistrar.hpp"

#include <sol/sol.hpp>

using namespace ogdf;

namespace engine::registration
{
    void register_classes(sol::state& lua)
    {
        ClassRegistrar<Graph>(lua, "Graph").ctor<>();

        ClassRegistrar<GraphAttributes>(lua, "GraphAttributes").ctor<const Graph&>();

        ClassRegistrar<SugiyamaLayout>(lua, "SugiyamaLayout")
            .ctor<>()
            .gc()
            .func("setLayout",
                  sol::resolve<void(HierarchyLayoutModule * pLayout)>(&SugiyamaLayout::setLayout))
            .func("call", sol::resolve<void(GraphAttributes & AG)>(&SugiyamaLayout::call));

        ClassRegistrar<HierarchyLayoutModule>(lua, "HierarchyLayoutModule");

        ClassRegistrar<OptimalHierarchyLayout>(lua, "OptimalHierarchyLayout")
            .ctor<>()
            .base<HierarchyLayoutModule>()
            .gc()
            .func("layerDistance",
                  sol::resolve<void(double x)>(&OptimalHierarchyLayout::layerDistance))
            .func("nodeDistance",
                  sol::resolve<void(double x)>(&OptimalHierarchyLayout::nodeDistance))
            .func("weightBalancing",
                  sol::resolve<void(double w)>(&OptimalHierarchyLayout::weightBalancing));
    }

    void register_global_functions(sol::state& lua)
    {
        FunctionRegistrar(lua)
            .func("read", [](GraphAttributes& GA, Graph& G, const string& filename) -> bool
                  { return GraphIO::read(GA, G, filename); })
            .func("write", [](const GraphAttributes& GA, const string& filename) -> bool
                  { return GraphIO::write(GA, filename); });
    }
} // namespace engine::registration
