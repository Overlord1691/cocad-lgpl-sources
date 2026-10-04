# ---------------------------------------------------------------------------
# planegcs -- FreeCAD's 2D geometric constraint solver, vendored (ADR 0005, ADR 0035).
#
# LGPL-2.1. This target lives inside core/cocad-core and is linked into the cocad-core
# executable only; nothing under apps/ or packages/ may reach it (CLAUDE.md rule 2).
#
# SHARED, NOT STATIC, AND THAT IS A LICENCE OBLIGATION (legal audit 2026-08-29, Katherine's
# approval; ADR 0296). LGPL-2.1 s6(b) discharges the user's relink right the moment a replaceable
# DLL ships -- the same mechanism, notices section and test story as OCCT's TK*.dll set. The
# alternative is publishing cocad-core's own object files (s6(a)) per release, which Katherine
# rejected as an ongoing obligation. Two rules follow, and both are load-bearing:
#   * Do not make this STATIC again for startup time, installer size or any other reason --
#     that re-opens the s6(a) obligation. Superseding ADR + Josh/Katherine first.
#   * The DLL must stay GENUINELY REPLACEABLE: no whole-program optimisation, no /GL, no LTCG,
#     no INTERPROCEDURAL_OPTIMIZATION across this boundary -- inlining solver code into the
#     caller is what would make the s6(b) argument evaporate while every test stays green.
#
# Deliberately NOT linked against cocad_warnings. Third-party code keeps whatever settings its
# own authors chose -- the point of that INTERFACE target is that CoCAD's flags apply to CoCAD's
# code. Turning /W4 /permissive- or -Wconversion loose on 12k lines of vendored solver would
# produce hundreds of diagnostics nobody in this repository can act on, and the one warning that
# mattered would be invisible among them. This is not the banned "-Wno- to clear a diagnostic":
# no flag is being suppressed on CoCAD's own code, and no diagnostic is being silenced -- the
# vendored files are simply compiled the way upstream compiles them.
# ---------------------------------------------------------------------------

add_library(planegcs SHARED
    App/planegcs/Constraints.cpp
    App/planegcs/GCS.cpp
    App/planegcs/Geo.cpp
    App/planegcs/SubSystem.cpp
    App/planegcs/qp_eq.cpp)

# PUBLIC so a consumer writes #include <planegcs/GCS.h>; the shim directory is PRIVATE because
# <FCConfig.h>, <Base/Console.h> and <boost_graph_adjacency_list.hpp> are resolved while
# compiling these sources and are no part of the solver's interface.
target_include_directories(planegcs
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/App"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(planegcs PUBLIC Eigen3::Eigen Boost::graph Boost::math)

target_compile_features(planegcs PUBLIC cxx_std_20)

# WINDOWS_EXPORT_ALL_SYMBOLS covers the solver's functions (the vendored sources carry no
# dllexport and may not be edited -- the pristine check pins them byte-for-byte). It does NOT
# cover data symbols or class statics; if the bridge ever references one, the link of
# cocad_core_lib's consumers fails with LNK2001 and the answer is a shim in src/sketch/, never
# an edit under App/. RUNTIME_OUTPUT_DIRECTORY puts the DLL beside cocad-core.exe (the
# multi-config generator appends the per-config subdirectory itself); the test executable gets
# a post-build copy in tests/CMakeLists.txt.
set_target_properties(planegcs PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    FOLDER "third_party")

if(MSVC)
    # Two settings, both about correctness rather than warnings:
    #   /bigobj  -- Constraints.cpp and GCS.cpp exceed the default 65,279-section object limit
    #               once Eigen's templates are instantiated. Without it the compiler stops with
    #               C1128 and names no line.
    #   /utf-8   -- the vendored sources carry UTF-8 copyright headers. Without it MSVC reads
    #               them in the ANSI codepage and warns (C4819) on files we do not own.
    target_compile_options(planegcs PRIVATE /bigobj /utf-8)
endif()
