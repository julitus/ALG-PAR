#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LIMIT 20

int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int count = 0;
  int partner_rank = (rank + 1) % 2;

  while (count < LIMIT) {
    if (rank == count % 2) {
      count++;
      MPI_Send(&count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("%d esta enviando e incrementando count %d a %d\n",
             rank, count, partner_rank);
      //sleep(1);
    } else {
      MPI_Recv(&count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      printf("%d esta recibiendo count %d de %d\n",
             rank, count, partner_rank);
      //sleep(1);
    }
  }
  MPI_Finalize();
  return 0;
}