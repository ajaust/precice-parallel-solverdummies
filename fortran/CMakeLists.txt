cmake_minimum_required(VERSION 3.10.2)
project(SolverdummyFortranParallel LANGUAGES C Fortran DESCRIPTION "preCICE Parallel Fortran solver dummy")

find_package(precice REQUIRED CONFIG)
find_package(MPI REQUIRED)

add_executable(solverdummy-fortran-parallel solverdummy-parallel-fortran.f90)
target_link_libraries(solverdummy-fortran-parallel PRIVATE precice::precice)
target_link_libraries(solverdummy-fortran-parallel PRIVATE MPI::MPI_Fortran)
