# cmake ../ -DCMAKE_BUILD_TYPE=Debug

cmake_minimum_required(VERSION 3.1)
project(test_cut_by_plane)

if(NOT POLICY CMP0070 AND POLICY CMP0053)
  # Only set CMP0053 to OLD with CMake<3.10, otherwise there is a warning.
  cmake_policy(SET CMP0053 OLD)
endif()
if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
################################################################################
add_definitions("-Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)

add_definitions("-g") # for profilling with vtune
add_link_options("-g")

## For profilling with gprof
# add_definitions("-pg")
# add_link_options("-pg")
################################################################################
find_package(CGAL REQUIRED COMPONENTS Qt5)
find_package(Qt5 REQUIRED COMPONENTS Script OpenGL Svg Widgets)

add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)

# Option allowing to profile each operation of the demo (cout times on stdout).
add_definitions(-DCGAL_PROFILE_LCC_DEMO)

## To add an is_valid test after each operation (only in debug mode)
# add_definitions("-DCGAL_CMAP_TEST_VALID_REMOVALS")
# add_definitions("-DCGAL_CMAP_TEST_VALID_CONTRACTIONS")
# add_definitions("-DCGAL_CMAP_TEST_VALID_INSERTIONS")

## To add expensive tests
# add_definitions("-DCGAL_CHECK_EXPENSIVE")
################################################################################
include_directories(tools)
include_directories(BEFORE SYSTEM ${CMAKE_SOURCE_DIR})

set(src_tools
  tools/lcc_creations.h
  tools/Linear_cell_complex_cut_operation.h
  )
add_library(lcc_tools_lib SHARED ${src_tools})
target_link_libraries(lcc_tools_lib PUBLIC
  CGAL::CGAL
  CGAL::CGAL_Qt5)
if (ASSIMP_FOUND)
  target_link_libraries(lcc_tools_lib PUBLIC assimp)
endif (ASSIMP_FOUND)
################################################################################

add_executable(cut-by-plane progs/cut-by-plane.cpp ${src_tools})
target_link_libraries(cut-by-plane PUBLIC lcc_tools_lib)
################################################################################