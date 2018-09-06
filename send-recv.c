#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank;
  int a;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    a = 123;
    MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (rank == 1) {
    a = 456;
    MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  printf("Process %d : a = %d\n", rank, a);
  
  MPI_Finalize();
  return 0;
}