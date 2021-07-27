cmake_minimum_required(VERSION 3.10.2)
project(SolverdummyCPPParallel LANGUAGES CXX DESCRIPTION "preCICE CPP solver dummy")

find_package(precice REQUIRED CONFIG)
find_package(MPI REQUIRED)

add_executable(solverdummy-cpp-parallel solverdummy-cpp-parallel.cpp)

set_target_properties(solverdummy-cpp-parallel PROPERTIES CXX_STANDARD 11)

target_link_libraries(solverdummy-cpp-parallel PRIVATE precice::precice)
target_link_libraries(solverdummy-cpp-parallel PRIVATE MPI::MPI_CXX)


