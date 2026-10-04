# Builds planegcs.dll on its own, exactly as CoCAD's core builds it, so you can replace the
# planegcs.dll that ships beside cocad-core.exe (LGPL-2.1 section 6).
#
# Uses the same toolchain CoCAD uses: vcpkg (manifest mode, vcpkg.json beside this file) for
# Eigen and Boost's header-only graph and math libraries, MSVC 2022 x64.
#
#   cmake -S . -B build -G "Visual Studio 17 2022" -A x64 ^
#         -DCMAKE_TOOLCHAIN_FILE=<vcpkg>/scripts/buildsystems/vcpkg.cmake
#   cmake --build build --config Release
#
# The result is build/Release/planegcs.dll. Copy it over the one in the CoCAD install folder.
cmake_minimum_required(VERSION 3.28)
project(planegcs-standalone LANGUAGES CXX)

find_package(Eigen3 CONFIG REQUIRED)
find_package(boost_graph CONFIG REQUIRED)
find_package(boost_math CONFIG REQUIRED)

# The directory CoCAD vendors, unchanged (its own CMakeLists.txt defines the `planegcs` target).
add_subdirectory(../planegcs planegcs)
