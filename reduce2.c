#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, a;
  int root = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  a = rank * 10;
  MPI_Allreduce(&a, &a, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("Value [a] of rank (%d) : %d\n", rank, a);
  
  MPI_Finalize();
  return 0;
}