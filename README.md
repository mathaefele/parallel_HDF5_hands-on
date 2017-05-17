# Parallel HDF5 hands-on
Slides and hands on material in C and fortran90 for learning parallel HDF5


*Environment required on the poincare machine*

module load gnu/4.7.2 openmpi/1.6.3_gnu47 hdf5/1.8.10_gnu47_openmpi
make

*To submit the execution of the application on the poincare machine*
llsubmit job_poincare

*Exercices*
1. Parallel multi files: all MPI ranks write their whole memory in separate file (provided in phdf5-1)
2. Serialized: each rank opens the same file (rank 0 has to create it first) and writes its data one after the other
  2.1 Data written as separate datasets => One file with multiple datasets
  2.2 Data written in the same dataset => One file with one single dataset
3. Parallel single file: specific HDF5 parameters given at open and write time to let MPI-IO manage the concurrent file access
