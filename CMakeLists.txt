# We want to use clang instead of gcc due to some linking problems
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)

# We need modern c++
set(CMAKE_CXX_STANDARD "14")
set(CMAKE_C_STANDARD "99")

cmake_minimum_required(VERSION 3.5)
project(AdvancedTessellatedMachine)
include(InstallRequiredSystemLibraries)

# Our current version
set(ATM_VERSION_MAJOR 0)
set(ATM_VERSION_MINOR 0)
set(ATM_VERSION_PATCH 0)

# Adding module paths
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/usr/lib64/cmake/glfw/")

# Finding external dependencies
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost 1.60 REQUIRED filesystem)

# Including the source files
include(src/CMakeLists.txt)

# including cpack
include(CTestInfo.cmake)
include(CPackInfo.cmake)
