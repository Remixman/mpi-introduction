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

  if (rank == root) {
    strcpy(s, "STRING IN PROCESS 0");
    for (i = 1; i < size; ++i) {
      MPI_Send(s, count, MPI_CHAR, i, 100, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(s, count, MPI_CHAR, root, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Received message in process (%d) : \"%s\"\n", rank, s);
  }
  
  MPI_Finalize();
  return 0;
}