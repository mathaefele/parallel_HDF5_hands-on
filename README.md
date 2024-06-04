# Parallel HDF5 hands-on
Slides and hands on material in C and fortran90 for learning parallel HDF5


*Environment required on the ruche machine with the installed modules*

Load the following modules
```bash

module load gcc/11.2.0/gcc-4.8.5
module load hdf5/1.12.0/gcc-11.2.0-openmpi

*To submit the execution of the application on the ruche cluster*
```bash
sbatch job
```


*Exercices*
1. Parallel multi files: all MPI ranks write their whole memory in separate file (provided in phdf5-1)
2. Serialized: each rank opens the same file (rank 0 has to create it first) and writes its data one after the other
  2.1 Data written as separate datasets => One file with multiple datasets
  2.2 Data written in the same dataset => One file with one single dataset
3. Parallel single file: specific HDF5 parameters given at open and write time to let MPI-IO manage the concurrent file access
