#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
//#include <semaphore.h>

long thread_count;
long long n;
double sum;
//sem_t sem;

void* Thread_sum(void* rank) {
  long my_rank = (long) rank;
  double factor;
  long long i;
  long long n_n = n/thread_count;
  long long first = n_n*my_rank;
  long long last = first + n_n;
  //double my_sum = 0.0;

  if (first % 2 == 0)
    factor = 1.0;
  else
    factor = -1.0;

  for (i = first; i < last; i++, factor = -factor) {
    //my_sum += factor/(2*i+1);
    sum += factor/(2 * i + 1);
  }
  //sem_wait(&sem);
  //sum += my_sum;
  //sem_post(&sem);

  return NULL;
}

int main(int argc, char* argv[]) {

  if(argc != 3){
    printf("Los arumentos son: <process_number> <number_terms>\n");
    return 0;
  }

  long th;
  pthread_t* th_handles;
  double start, finish;

  thread_count = atoi(argv[1]);
  n = atoi(argv[2]);

  th_handles = (pthread_t*) malloc (thread_count * sizeof(pthread_t)); 
  //sem_init(&sem, 0, 1);

  sum = 0.0;
  for (th = 0; th < thread_count; th++)  
    pthread_create(&th_handles[th], NULL, Thread_sum, (void*)th);  

  for (th = 0; th < thread_count; th++) 
    pthread_join(th_handles[th], NULL);

  sum = 4.0 * sum;

  printf("PI obtenido = %.15f\n", sum);
  printf("PI = %.15f\n", 4.0 * atan(1.0));

  //sem_destroy(&sem);
  free(th_handles);
  return 0;
}