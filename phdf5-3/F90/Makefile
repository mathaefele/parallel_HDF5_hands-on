##############################################################################
## Makefile --- Hands on HDF5 
##
## Author          : Matthieu Haefele  
##                   Maison de la Simulation 
##                   matthieu.haefele@maisondelasimulation
## 
###############################################################################

MPIF90=mpif90

FFLAGS_HDF5= -I$(HDF5_INC_DIR)
LDFLAGS_HDF5= -L$(HDF5_LIB_DIR)  -lhdf5hl_fortran -lhdf5_fortran -lhdf5_hl -lhdf5 -lz

phdf5_ex3 : phdf5_ex3.o
	$(MPIF90) $(FFLAGS_HDF5) phdf5_ex3.o -o $@ $(LDFLAGS_HDF5)


##### RULES #####

.SUFFIXES: .f90

.f90.o:
	$(MPIF90) $(FFLAGS_HDF5) -c $< 



##### TARGETS #####


clean: 
	@-rm -f phdf5_ex3
	@-rm -f *.o *~ core* *.h5 *.log *.err my_env

