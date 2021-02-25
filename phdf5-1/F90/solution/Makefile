##############################################################################
## Makefile --- Hands on HDF5 
##
## Author          : Matthieu Haefele  
##                   Maison de la Simulation 
##                   matthieu.haefele@maisondelasimulation
## 
###############################################################################
include ../../../arch/make_inc

phdf5_ex1 : phdf5_ex1.o
	$(MPIF90) $(FFLAGS_HDF5) phdf5_ex1.o -o $@ $(LDFLAGS_HDF5)


##### RULES #####

.SUFFIXES: .f90

.f90.o:
	$(MPIF90) $(FFLAGS_HDF5) -c $< 



##### TARGETS #####


clean: 
	@-rm -f phdf5_ex1
	@-rm -f *.o *~ core* *.h5 *.log *.err my_env

