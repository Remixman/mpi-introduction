#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, a, b;
  int root = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  a = rank * 10;
  MPI_Reduce(&a, &b, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
  printf("Value [b] of rank (%d) : %d\n", rank, b);
  
  MPI_Finalize();
  return 0;
}