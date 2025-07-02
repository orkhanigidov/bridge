// Auto-generated file. Do not edit manually!

#include "engine/lua_bindings/class_registrar.hpp"
#include "engine/lua_bindings/function_registrar.hpp"
#include "ogdf/basic/GraphAttributes.h"
#include "ogdf/fileformats/GraphIO.h"
#include "ogdf/layered/OptimalHierarchyLayout.h"
#include "ogdf/layered/SugiyamaLayout.h"

#include <sol/sol.hpp>

using namespace ogdf;

namespace engine::lua_bindings
{
    void register_classes(sol::state& lua)
    {
        lua_bindings::class_registrar<Graph>(lua, "Graph").add_constructor<>();

        class_registrar<GraphAttributes>(lua, "GraphAttributes").add_constructor<const Graph&>();

        class_registrar<SugiyamaLayout>(lua, "SugiyamaLayout")
            .add_constructor<>()
            .enable_garbage_collector()
            .add_function("setLayout", sol::resolve<void(HierarchyLayoutModule * pLayout)>(
                                           &SugiyamaLayout::setLayout))
            .add_function("call", sol::resolve<void(GraphAttributes & AG)>(&SugiyamaLayout::call));

        class_registrar<HierarchyLayoutModule>(lua, "HierarchyLayoutModule");

        class_registrar<OptimalHierarchyLayout>(lua, "OptimalHierarchyLayout")
            .add_constructor<>()
            .add_base_class<HierarchyLayoutModule>()
            .enable_garbage_collector()
            .add_function("layerDistance",
                          sol::resolve<void(double x)>(&OptimalHierarchyLayout::layerDistance))
            .add_function("nodeDistance",
                          sol::resolve<void(double x)>(&OptimalHierarchyLayout::nodeDistance))
            .add_function("weightBalancing",
                          sol::resolve<void(double w)>(&OptimalHierarchyLayout::weightBalancing));
    }

    void register_global_functions(sol::state& lua)
    {
        function_registrar(lua)
            .add_function("read", [](GraphAttributes& GA, Graph& G, const string& filename) -> bool
                          { return GraphIO::read(GA, G, filename); })
            .add_function("write", [](const GraphAttributes& GA, const string& filename) -> bool
                          { return GraphIO::write(GA, filename); });
    }
} // namespace engine::lua_bindings