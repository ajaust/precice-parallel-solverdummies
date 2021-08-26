cmake_minimum_required(VERSION 3.10.2)
project(SolverdummyCParallel LANGUAGES C DESCRIPTION "Parallel preCICE C solver dummy")

find_package(precice REQUIRED CONFIG)
find_package(MPI REQUIRED)

add_executable(solverdummy-c-parallel solverdummy-c-parallel.c)

set_property(TARGET solverdummy-c-parallel PROPERTY C_STANDARD 99)

target_link_libraries(solverdummy-c-parallel PRIVATE precice::precice)
target_link_libraries(solverdummy-c-parallel PRIVATE MPI::MPI_C)
