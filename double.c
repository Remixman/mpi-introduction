#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int size, rank, i, numbers[200];
  int num_per_proc, *sub_nums;
  int root = 0, sub_sum = 0, sum = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  num_per_proc = 200 / size;

  if (rank == root) {
    for (i = 0; i < 200; ++i) 
      numbers[i] = i + 1;
  }

  sub_nums = (int*) malloc(num_per_proc * sizeof(int));
  MPI_Scatter(numbers, num_per_proc, MPI_INT, sub_nums, num_per_proc, MPI_INT, root, MPI_COMM_WORLD);

  for (i = 0; i < num_per_proc; i++)
    sub_nums[i] *= 2;

  MPI_Gather(sub_nums, num_per_proc, MPI_INT, numbers, num_per_proc, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    for (i = 0; i < 200; i++)
      printf("%5d");
    printf("\n");
  }

  free(sub_nums);

  MPI_Finalize();
  return 0;
}