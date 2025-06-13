// Auto-generated file. Do not edit manually!

#include "ogdf/basic/GraphAttributes.h"
#include "ogdf/fileformats/GraphIO.h"
#include "ogdf/layered/OptimalHierarchyLayout.h"
#include "ogdf/layered/SugiyamaLayout.h"

#include <ogdf/basic/Graph.h>
#include <sol/sol.hpp>

using namespace ogdf;

void register_with_sol2(sol::state& lua)
{
    auto ogdf_table = lua["ogdf"].get_or_create<sol::table>();

    ogdf_table.new_usertype<Graph>("Graph", sol::call_constructor, sol::constructors<Graph>());

    ogdf_table.new_usertype<GraphAttributes>("GraphAttributes", sol::call_constructor,
                                             sol::constructors<GraphAttributes(const Graph&)>());

    ogdf_table.set_function(
        "read", sol::overload([](GraphAttributes& GA, Graph& G, const string& filename) -> bool
                              { return GraphIO::read(GA, G, filename); }));

    ogdf_table.new_usertype<SugiyamaLayout>(
        "SugiyamaLayout", sol::call_constructor, sol::constructors<SugiyamaLayout()>(), "setLayout",
        &SugiyamaLayout::setLayout, "call",
        sol::overload([](SugiyamaLayout& layout, GraphAttributes& GA) { return layout.call(GA); }));

    ogdf_table.new_usertype<OptimalHierarchyLayout>(
        "OptimalHierarchyLayout", sol::call_constructor,
        sol::constructors<OptimalHierarchyLayout()>(), "layerDistance",
        sol::overload(static_cast<void (OptimalHierarchyLayout::*)(double)>(
            &OptimalHierarchyLayout::layerDistance)),
        "nodeDistance",
        sol::overload(static_cast<void (OptimalHierarchyLayout::*)(double)>(
            &OptimalHierarchyLayout::nodeDistance)),
        "weightBalancing",
        sol::overload(static_cast<void (OptimalHierarchyLayout::*)(double)>(
            &OptimalHierarchyLayout::weightBalancing)));

    ogdf_table.set_function("write",
                            sol::overload([](GraphAttributes& GA, const string& filename) -> bool
                                          { return GraphIO::write(GA, filename); }));
}