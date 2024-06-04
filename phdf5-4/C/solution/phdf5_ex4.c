/* ###################################################################
   #                       HDF5 hands on                             #
   #  Created : March 2015                                           #
   #                                                                 #
   #  Author:                                                        #
   #     Matthieu Haefele                                            #
   #     matthieu.haefele@maisondelasimulation.fr                    #
   #     CNRS/Maison de la Simulation                                #
   #                                                                 #
   ################################################################### */

#include <mpi.h>
#include <hdf5.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#define NX     16
#define NY     32
#define RANK   2

void print(int* data, int size_x, int size_y)
{
  int i,j;

  for(j=0; j < size_y; j++)
  {
    for(i=0; i < size_x; i++)
      printf("%d ", data[i + (j)*size_x]);
    printf("\n");
  }
    printf("#######\n");
}

int* init(int mpi_id, int size, int* sx, int* sy, int* ox, int* oy, int* sx_array, int* sy_array)
{
  int i,j;
  int* data;
  int nb_proc_per_dim = 1;
  if(size == 4)
  	nb_proc_per_dim = 2;
  else if(size == 16)
  	nb_proc_per_dim = 4;
  //int nb_proc_per_dim = (int) sqrt(size);
  int nb_proc_per_dim = (int) sqrt(size);
  int size_x = NX/nb_proc_per_dim + 2;
  int size_y = NY/nb_proc_per_dim + 2;
  int offset_x = (mpi_id%nb_proc_per_dim) * (size_x-2);
  int offset_y = ((int) mpi_id/nb_proc_per_dim) * (size_y-2);

  data = malloc(size_x*size_y*sizeof(int));

  for(j=-1; j < size_y-1; j++)
    for(i=-1; i < size_x-1; i++)
    {
      if(i+offset_x<0 || j+offset_y < 0 || i+offset_x >= NX || j+offset_y >= NY)
        data[i+1 + (j+1)*size_x] = 0;
      else
        data[i+1 + (j+1)*size_x] = i+offset_x + (j+offset_y)*NX;
    }

  *sx = size_x-2;
  *sy = size_y-2;
  *ox = offset_x;
  *oy = offset_y;
  *sx_array = size_x;
  *sy_array = size_y;
  return data;
}

int main (int argc, char** argv)
{
  hid_t       file, dataset, dataspace, filespace, memspace, plist_id1, plist_id2;
  hsize_t     dimsf[RANK], start[RANK], count[RANK];
  herr_t      status;
  int*        data;
  int mpi_id, size;
  int size_x, size_y, i, offset_x, offset_y, size_x_array, size_y_array;
  char name[128];
  
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &mpi_id);
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  //printf( "Hello from process %d of %d\n", mpi_id, size );

  if(size != 1 && size != 4 && size != 16)
  {
    if(mpi_id==0)
      printf("Error: Authorized number of process is 1, 4 or 16, exiting...\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }


  //Data initialization
  data = init(mpi_id, size, &size_x, &size_y, &offset_x, &offset_y, &size_x_array, &size_y_array);
  printf("id=%d size_x=%d size_y=%d offset_x=%d offset_y=%d\n", mpi_id, size_x, size_y, offset_x,offset_y);
  
  //HDF5 file creation
  plist_id1 = H5Pcreate(H5P_FILE_ACCESS);
  H5Pset_fapl_mpio(plist_id1, MPI_COMM_WORLD, MPI_INFO_NULL);
  file = H5Fcreate("example.h5", H5F_ACC_TRUNC, H5P_DEFAULT, plist_id1);

  //Dataspace creation
  dimsf[0] = NY;
  dimsf[1] = NX;
  dataspace = H5Screate_simple(RANK, dimsf, NULL); 

  //Dataset creation 
  dataset = H5Dcreate(file, "IntArray", H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  //Dataspace for the selection needed in the file
  count[0] = size_y;
  count[1] = size_x;
  start[0] = offset_y;
  start[1] = offset_x;
  filespace = H5Screate_simple(RANK, dimsf, NULL);
  status = H5Sselect_hyperslab (filespace, H5S_SELECT_SET, start, NULL, count, NULL);

  count[0] = size_y_array;
  count[1] = size_x_array;
  memspace = H5Screate_simple(RANK, count, NULL);
  count[0] = size_y;
  count[1] = size_x;
  start[0] = 1;
  start[1] = 1;
  status = H5Sselect_hyperslab (memspace, H5S_SELECT_SET, start, NULL, count, NULL);

  //Actual data IO
  plist_id2 = H5Pcreate(H5P_DATASET_XFER);
  H5Pset_dxpl_mpio(plist_id2, H5FD_MPIO_COLLECTIVE);
  status = H5Dwrite(dataset, H5T_NATIVE_INT, memspace, filespace, plist_id2, data);

  //Closing all opened HDF5 objects
  H5Sclose(filespace);
  H5Sclose(memspace);
  H5Sclose(dataspace);
  H5Dclose(dataset);
  H5Fclose(file);
  
  MPI_Finalize();

  return 0;
}     
