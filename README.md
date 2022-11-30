# Parallel solver dummies for preCICE

This repository currently contains four solver dummies for the coupling library [preCICE](https://github.com/precice/precice) that can be executed in parallel. The solver dummies can also be mixed with each other, i.e., each solver can play any role in the coupling!

Current solver dummies included
- C solver dummy: `c/solverdummy-c-parallel.cpp`
- C++ solver dummy: `cpp/solverdummy-cpp-parallel.cpp`
- Fortran solver dummy: `fortran/solverdummy-fortran-parallel.f90`
- Julia solver dummy: `julia/solverdummy-julia-parallel.jl`
- Python solver dummy: `python/solverdummy-python-parallel.py`


## Requirements

**All dummies**
- preCICE v2 (only tested with v2.2.0)

**C dummy**
- CMake
- A C compiler
- MPI (only tested with OpenMPI)

**C++ dummy**
- CMake
- A C++ compiler
- MPI (only tested with OpenMPI)

**Fortran dummy**
- CMake
- A Fortran compiler
- MPI (only tested with OpenMPI)

**Julia dummy**
- Julia `v1.6` or above
- [Julia bindings for preCICE](https://github.com/precice/PreCICE.jl)

**Python dummy additionally needs**
- Python 3
- mpi4py
- [Python bindings for preCICE](https://github.com/precice/python-bindings)

If you have preCICE and the Python bindings installed, you should have all requirements installed.

## Compilation

### C

Compile the C solver dummy and copy it back to the `c` directory:

```
cd c
mkdir build
cd build
cmake ..
make
cp solverdummy-c-parallel ..
cd ..
```

### C++

Compile the C++ solver dummy and copy it back to the `cpp` directory:

```
cd cpp
mkdir build
cd build
cmake ..
make
cp solverdummy-cpp-parallel ..
cd ..
```

### Fortran

Compile the Fortran solver dummy and copy it back to the `fortran` directory:

```
cd fortran
mkdir build
cd build
cmake ..
make
cp solverdummy-fortran-parallel ..
cd ..
```

## Test solvers

`N` and `M` refer to the number of ranks one want to use. Start with `1` in order to run the serial computation and increase `N` and `M` afterwards to have truly parallel computations.

Further information:
- `N` and `M` may be different numbers.
- The solver have to run in different shells or you have to send the first solver to the background.
- You pass the participant's name and the corresponding mesh name on the terminal. The name and mesh have to be one of the following
    - `SolverOne` and `MeshOne`
    - `SolverTwo` and `MeshTwo`
- Replace `mpirun` by the suitable MPI wrapper of your machine.
- Step into the directory of the solver you want to use, i.e. `cd cpp/` for the C++ solver.
- If you copy the solvers into other directories, please adapt the file `precice-config-parallel.xml` accordingly.

**Note** You can mix the different dummies arbitrarily with each other. Some examples are given below:
### Setup 1: C++ and C++

```
mpirun -n N ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n M ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverTwo
```

Example:
```
mpirun -n 4 ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n 2 ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverTwo
```

### Setup 2: Python and Python

```
mpirun -n N python3 solverdummy-python-parallel.py ../precice-config-parallel.xml SolverOne
mpirun -n M python3 solverdummy-python-parallel.py ../precice-config-parallel.xml SolverTwo
```

Example:
```
mpirun -n 4 python3 solverdummy-python-parallel.py ../precice-config-parallel.xml SolverOne
mpirun -n 2 python3 solverdummy-python-parallel.py ../precice-config-parallel.xml SolverTwo
```

### Setup 3: C++ and Python

```
mpirun -n N ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n M python3 solverdummy-python-parallel.py ../precice-config-parallel.xml SolverTwo
```

Example:
```
mpirun -n 4 ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n 2 python3 solverdummy-python-parallel.py ../precice-config-parallel.xml SolverTwo
```

### Setup 4: Fortran and C++

```
mpirun -n N ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n M ./solverdummy-fortran-parallel ../precice-config-parallel.xml SolverTwo
```

Example:
```
mpirun -n 4 ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n 2 ./solverdummy-fortran-parallel ../precice-config-parallel.xml SolverTwo
```

### Setup 4: C++ and C

```
mpirun -n N ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n N ./solverdummy-c-parallel ../precice-config-parallel.xml SolverOne
```

Example:
```
mpirun -n 4 ./solverdummy-cpp-parallel ../precice-config-parallel.xml SolverOne
mpirun -n 2 ./solverdummy-c-parallel ../precice-config-parallel.xml  SolverTwo
```

### Setup 5: Julia and Julia

The Julia solver dummy uses Julia's native parallelization and not the MPI wrapper.

You need to add `<master:sockets/>` to the `precice-config-parallel.xml` file in both participants.

Example:

```xml
<participant name="SolverOne">
    <master:sockets/>
    <use-mesh name="MeshOne" provide="yes"/>
    <write-data name="dataOne" mesh="MeshOne" />
    <read-data  name="dataTwo" mesh="MeshOne" />
</participant>
```

For convenience, the repository provides a `precice-config-parallel-julia.xml` configuration that can be used with the Julia solver dummy.

Run the dummies with

```shell
julia solverdummy-julia-parallel.jl N ../precice-config-parallel-julia.xml SolverOne
julia solverdummy-julia-parallel.jl M ../precice-config-parallel-julia.xml SolverTwo
```

Example:
```shell
julia solverdummy-julia-parallel.jl 4 ../precice-config-parallel-julia.xml SolverOne
julia solverdummy-julia-parallel.jl 2 ../precice-config-parallel-julia.xml SolverTwo
```

## Setup 6: Julia and C++

As with [Setup 5](#setup-5-julia-and-julia) you need to add `<master:sockets/>` to the `precice-config-parallel.xml` file in both participants or you can used the provided `precice-config-parallel-julia.xml` configuration.

```shell
mpirun -n N ./solverdummy-cpp-parallel ../precice-config-parallel-julia.xml SolverOne MeshOne
julia solverdummy-julia-parallel.jl M ../precice-config-parallel-julia.xml SolverTwo
```

Example:
```shell
mpirun -n 4 ./solverdummy-cpp-parallel ../precice-config-parallel-julia.xml SolverOne MeshOne
julia solverdummy-julia-parallel.jl 2 ../precice-config-parallel-julia.xml SolverTwo
```

## Remarks

- The codes are heavily based on the solver dummies of [preCICE](https://github.com/precice/precice) and the corresponding [Python bindings](https://github.com/precice/python-bindings).