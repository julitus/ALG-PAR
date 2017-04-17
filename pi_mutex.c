#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

#define GET_TIME(now) { \
  struct timeval t; \
  gettimeofday(&t, NULL); \
  now = t.tv_sec + t.tv_usec/1000000.0; \
}

long th_count;
long long n;
double sum;
int flag;
pthread_mutex_t mutex; 

void* Thread_sum(void* rank) {
  long n_rank = (long) rank;
  double factor;
  long long i;
  long long n_n = n/th_count;
  long long first = n_n*n_rank;
  long long last = first + n_n;
  double my_sum = 0.0;

  if (first % 2 == 0)
    factor = 1.0;
  else
    factor = -1.0;

  for (i = first; i < last; i++, factor = -factor) {
    my_sum += factor/(2 * i + 1);
  }

  pthread_mutex_lock(&mutex);
  sum += my_sum;
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(int argc, char* argv[]) {

  if(argc != 3){
    printf("Los arumentos son: <process_number> <number_terms>\n");
    return 0;
  }

  long th;
  pthread_t *th_handles;
  double start, finish;

  if (pthread_mutex_init(&mutex, NULL) != 0){
    printf("Inicializacion de Mutex fallida\n");
    return 1;
  }

  th_count = atoi(argv[1]);
  n = atoi(argv[2]);

  th_handles = (pthread_t*) malloc (th_count * sizeof(pthread_t)); 

  GET_TIME(start);
  sum = 0.0;
  flag = 0;
  for (th = 0; th < th_count; th++)  
    pthread_create(&th_handles[th], NULL, Thread_sum, (void*)th);  

  for (th = 0; th < th_count; th++) 
    pthread_join(th_handles[th], NULL);
  sum = 4.0 * sum;
  GET_TIME(finish);

  printf("PI obtenido = %.15f\n", sum);
  printf("PI = %.15f\n", 4.0 * atan(1.0));
  printf("Tiempo %lf segundos\n", finish - start);

  free(th_handles);
  return 0;
}

/*
gcc pi_mutex.c -lpthread
./a.out <process_number> <number_terms>
./a.out n 100000000
1   0.803
2   0.491
4   0.440
8   0.399
16  0.414
32  0.438
64  0.406
*/