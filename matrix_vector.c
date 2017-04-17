#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_INT 10;

int th_count, m, n;
int *A, *x, *y;

int *randomMatrix(int m, int n){
    srand(time(NULL));
    int *result = (int *) malloc(sizeof(int) * m * n);
    for(int i = 0; i < m * n; i++){
        result[i] = rand() % MAX_INT + 1;
    }
    return result;
}

int *randomVector(int n){
    srand(time(NULL));
    int *result = (int *) malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++){
        result[i] = rand() % MAX_INT + 1;
    }
    return result;
}

void printMatrix(int A[], int m, int n) {
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", A[i*n + j]);
        printf("\n");
    }
}

void printVector(int y[], int m) {
    int i;
    for (i = 0; i < m; i++)
        printf("%d ", y[i]);
    printf("\n");
}

void *multiplication_m_v(void* rank) {
    long n_rank = (long) rank;
    int i, j;
    int local_m = m / th_count; 

    for (i = n_rank * local_m; i < n_rank * local_m + local_m; i++) {
        y[i] = 0;
        for (j = 0; j < n; j++)
            y[i] += A[i * n + j] * x[j];
    }
    printf("Multiplicacion del thread %ld/%d terminado\n", n_rank, th_count);
    return NULL;
}

int main(int argc, char* argv[]) {
    if(argc != 4){
        printf("Los arumentos son: <process_number> <number_rows> <number_columns>\n");
        return 0;
    }

    long th;
    pthread_t* th_handles;

    th_count = atoi(argv[1]);
    m = atoi(argv[2]);
    n = atoi(argv[3]);
    th_handles = malloc(th_count * sizeof(pthread_t));

    A = randomMatrix(m, n);
    x = randomVector(n);
    y = malloc(m * sizeof(int));

    for (th = 0; th < th_count; th++)
        pthread_create(&th_handles[th], NULL, multiplication_m_v, (void*) th);

    for (th = 0; th < th_count; th++)
        pthread_join(th_handles[th], NULL);

    printVector(y, m);

    free(A); free(x); free(y); free(th_handles);

   return 0;
}