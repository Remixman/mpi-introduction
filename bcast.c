#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, i, count;
  int root = 0;
  char s[20];
  count = 20;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == root)
    strcpy(s, "STRING IN PROCESS 0");

  MPI_Bcast(s, 20, MPI_CHAR, root, MPI_COMM_WORLD);

  if (rank != root)
    printf("Received message in process (%d) : \"%s\"\n", rank, s);
  
  MPI_Finalize();
  return 0;
}