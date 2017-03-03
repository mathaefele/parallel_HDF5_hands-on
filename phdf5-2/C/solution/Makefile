##############################################################################
## Makefile --- Hands on HDF5 
##
## Author          : Matthieu Haefele  
##                   Maison de la Simulation 
##                   matthieu.haefele@maisondelasimulation
## 
###############################################################################


CC= mpicc
CCFLAGS_HDF5= -I$(HDF5_INC_DIR)
CLDFLAGS_HDF5= -L$(HDF5_LIB_DIR) -lhdf5_hl -lhdf5 -lz 

phdf5_ex2 : phdf5_ex2.o
	$(CC) $(CCFLAGS_HDF5) phdf5_ex2.o -o $@ $(CLDFLAGS_HDF5)


##### RULES #####

.SUFFIXES: .c 

.c.o:
	$(CC) $(CCFLAGS_HDF5) -c $< 


##### TARGETS #####



clean: 
	@-rm -f phdf5_ex2 
	@-rm -f *.o *~ core* *.h5 *.err *.log my_env

