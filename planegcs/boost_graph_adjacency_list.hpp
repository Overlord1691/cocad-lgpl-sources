// CoCAD shim for FreeCAD's src/boost_graph_adjacency_list.hpp.
//
// Upstream this header exists only to wrap <boost/graph/adjacency_list.hpp> in compiler-specific
// warning suppressions. CoCAD builds vendored third-party code without its own warning flags
// (the cocad_warnings INTERFACE target is linked into CoCAD targets and nothing else), so the
// suppressions are unnecessary and the wrapper is a plain forward.
#ifndef COCAD_PLANEGCS_SHIM_BOOST_GRAPH_ADJACENCY_LIST_HPP
#define COCAD_PLANEGCS_SHIM_BOOST_GRAPH_ADJACENCY_LIST_HPP

#include <boost/graph/adjacency_list.hpp>

#endif  // COCAD_PLANEGCS_SHIM_BOOST_GRAPH_ADJACENCY_LIST_HPP
