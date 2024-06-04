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
#define NX     16
#define NY     32
#define RANK   2

int* init(int mpi_id, int size, int* sx, int* sy, int* ox, int* oy)
{
  int i,j;
  int* data;
  int nb_proc_per_dim = 1;
  if(size == 4)
  	nb_proc_per_dim = 2;
  else if(size == 16)
  	nb_proc_per_dim = 4;
  //int nb_proc_per_dim = (int) sqrt(size);
  int size_x = NX/nb_proc_per_dim;
  int size_y = NY/nb_proc_per_dim;
  int offset_x = (mpi_id%nb_proc_per_dim) * size_x;
  int offset_y = ((int) mpi_id/nb_proc_per_dim) * size_y;

  data = malloc(size_x*size_y*sizeof(int));

  for(j=0; j < size_y; j++)
    for(i=0; i < size_x; i++)
      data[i + j*size_x] = i+offset_x + (j+offset_y)*NX;

  *sx = size_x;
  *sy = size_y;
  *ox = offset_x;
  *oy = offset_y;
  return data;
}

int main (int argc, char** argv)
{
  hid_t       file, dataset, dataspace, filespace, memspace;
  hsize_t     dimsf[RANK], start[RANK], count[RANK];
  herr_t      status;
  int*        data;
  int mpi_id, size;
  int size_x, size_y, i, offset_x, offset_y;
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
  data = init(mpi_id, size, &size_x, &size_y, &offset_x, &offset_y);
  printf("id=%d size_x=%d size_y=%d offset_x=%d offset_y=%d\n", mpi_id, size_x, size_y, offset_x,offset_y);
  
  if(mpi_id == 0)
  {
    //HDF5 file creation
    file = H5Fcreate("example.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    dimsf[0] = NY;
    dimsf[1] = NX;

    //Dataspace creation
    dataspace = H5Screate_simple(RANK, dimsf, NULL); 

    //Dataset creation 
    dataset = H5Dcreate(file, "IntArray", H5T_NATIVE_INT, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //Dataspace for the selection needed in the file
    count[0] = size_y;
    count[1] = size_x;
    start[0] = offset_y;
    start[1] = offset_x;
    memspace = H5Screate_simple(RANK, count, NULL);
    filespace = H5Screate_simple(RANK, dimsf, NULL);
    status = H5Sselect_hyperslab (filespace, H5S_SELECT_SET, start, NULL, count, NULL);

    //Actual data IO
    status = H5Dwrite(dataset, H5T_NATIVE_INT, memspace, filespace, H5P_DEFAULT, data);

    //Closing all opened HDF5 objects
    H5Sclose(filespace);
    H5Sclose(memspace);
    H5Sclose(dataspace);
    H5Dclose(dataset);
    H5Fclose(file);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  for(i=1; i < size; ++i)
  {
    if(i==mpi_id)
    {
      //HDF5 file creation
      file = H5Fopen("example.h5", H5F_ACC_RDWR, H5P_DEFAULT);
      dimsf[0] = NY;
      dimsf[1] = NX;

      //Dataset  opening
      dataset = H5Dopen(file, "IntArray", H5P_DEFAULT);

      //Dataspace for the selection needed in the file
      count[0] = size_y;
      count[1] = size_x;
      start[0] = offset_y;
      start[1] = offset_x;
      memspace = H5Screate_simple(RANK, count, NULL);
      filespace = H5Screate_simple(RANK, dimsf, NULL);
      status = H5Sselect_hyperslab (filespace, H5S_SELECT_SET, start, NULL, count, NULL);

      //Actual data IO
      status = H5Dwrite(dataset, H5T_NATIVE_INT, memspace, filespace, H5P_DEFAULT, data);

      //Closing all opened HDF5 objects
      H5Sclose(filespace);
      H5Sclose(memspace);
      H5Dclose(dataset);
      H5Fclose(file);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  
  MPI_Finalize();

  return 0;
}     
