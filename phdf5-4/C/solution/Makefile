##############################################################################
## Makefile --- Hands on HDF5 
##
## Author          : Matthieu Haefele  
##                   Maison de la Simulation 
##                   matthieu.haefele@maisondelasimulation
## 
###############################################################################
include ../../../arch/make_inc

phdf5_ex4 : phdf5_ex4.o
	$(CC) $(CCFLAGS_HDF5) phdf5_ex4.o -o $@ $(CLDFLAGS_HDF5) $(LIBS)


##### RULES #####

.SUFFIXES: .c 

.c.o:
	$(CC) $(CCFLAGS_HDF5) -c $< 


##### TARGETS #####



clean: 
	@-rm -f phdf5_ex4 
	@-rm -f *.o *~ core* *.h5 *.err *.log my_env

