#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void synchronous_send(int rank){

  int num;
  if (rank == 0) {
    num = -1;
    printf("0 send >>\n");
    MPI_Ssend(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 finished\n");
  } else if (rank == 1) {
    printf("1 received <<\n");
    MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("1 finished\n");
  }

}

void ready_send(int rank){

  int num;
  if (rank == 0) {
    num = -1;
    printf("0 send >>\n");
    MPI_Rsend(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 finished\n");
  } else if (rank == 1) {
    printf("1 received <<\n");
    MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("1 finished\n");
  }

}

void standard_send(int rank){

  int num;
  if (rank == 0) {
    num = -1;
    printf("0 send >>\n");
    MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 finished\n");
  } else if (rank == 1) {
    printf("1 received<<\n");
    MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("1 finished>>\n");
  }

}

int main(int argc, char** argv) {
  
  MPI_Init(NULL, NULL);
  
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 2) {
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  synchronous_send(rank);
  //ready_send(rank);
  //standard_send(rank);
  
  MPI_Finalize();
  return 0;
}