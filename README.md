# Parallel HDF5 hands-on
Slides and hands on material in C and fortran90 for learning parallel HDF5


*Environment required on the plafrim machine with guix*

If not yet done during HDF5_hands-on, first initialize guix at the current version
```bash
guix pull # The first one triggers an error
guix pull # The second one is pretty long...
```

Then compile exercices inside the following guix shell
```bash
guix shell openmpi hdf5-parallel-openmpi@1.10.7 hdf5-parallel-openmpi@1.10.7:fortran coreutils gfortran-toolchain gcc-toolchain
```

*To submit the execution of the application on the plafrim machine*
```bash
sbatch job_plafrim
```




*Exercices*
1. Parallel multi files: all MPI ranks write their whole memory in separate file (provided in phdf5-1)
2. Serialized: each rank opens the same file (rank 0 has to create it first) and writes its data one after the other
  2.1 Data written as separate datasets => One file with multiple datasets
  2.2 Data written in the same dataset => One file with one single dataset
3. Parallel single file: specific HDF5 parameters given at open and write time to let MPI-IO manage the concurrent file access
