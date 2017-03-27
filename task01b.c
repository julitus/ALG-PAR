/*#include <sys/types.h>
#include <unistd.h>*/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )

typedef int NUM;

struct Matrix
{
	int r, c;
	NUM **m;
};

void print(struct Matrix m) {
	int i, j;
	for (i = 0; i < m.r; ++i) {
		for (j = 0; j < m.c; ++j) 
		  	printf("%d\t", m.m[i][j]);
		printf("\n");
	}
}

void generate(struct Matrix *m) {
	int i, j;
	m->m = malloc(m->r * sizeof(NUM*));
	for (i = 0; i < m->r; ++i) {
		m->m[i] = malloc(m->c * sizeof(NUM));
		for (j = 0; j < m->c; ++j)
		  	m->m[i][j] = (rand() % 10);
	}
}

struct Matrix three_nested_loop(struct Matrix a, struct Matrix b) {
	struct Matrix m;
	m.r = a.r; m.c = b.c;
	int i, j, k;
	m.m = malloc(m.r * sizeof(NUM*));
	for (i = 0; i < m.r; ++i) {
		m.m[i] = malloc(m.c * sizeof(NUM));
		for (j = 0; j < m.c; ++j) {
			m.m[i][j] = 0;
			for (k = 0; k < a.c; ++k)
		  		m.m[i][j] = m.m[i][j] + (a.m[i][k] * b.m[k][j]);
		}
	}
	return m;
}

struct Matrix bloqued_version(struct Matrix a, struct Matrix b, int blocks) {
	struct Matrix m;
	m.r = a.r; m.c = b.c;
	int i, j, k, ii, jj, kk;
	m.m = malloc(m.r * sizeof(NUM*));
	/*for(ii = 0; ii < (m.r / blocks); ++ii) {
		for(jj = 0; jj < (m.c / blocks); ++jj) {
			for(kk = 0; kk < (a.c / blocks); ++kk) {
				for(i = ii; i < ii + blocks; ++i) {
					m.m[i] = malloc(m.c * sizeof(NUM));
					for(j = jj; j < jj + blocks; ++j) {
						m.m[i][j] = 0;
						for(k = kk; k < kk + blocks; ++k)
							m.m[i][j] = m.m[i][j] + (a.m[i][k] * b.m[k][j]);
					}
				}
			}
		}
	}*/
	for(ii = 0; ii < m.r; ii += blocks) {
		for(jj = 0; jj < m.c; jj +=  blocks) {
			for(kk = 0; kk < a.c; kk += blocks) {
				for(i = ii; i < ii + blocks; ++i) {
					m.m[i] = malloc(m.c * sizeof(NUM));
					for(j = jj; j < jj + blocks; ++j) {
						m.m[i][j] = 0;
						for(k = kk; k < kk + blocks; ++k)
							m.m[i][j] = m.m[i][j] + (a.m[i][k] * b.m[k][j]);
					}
				}
			}
		}
	}
	return m;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	int n = 10, blk = 2;
	struct Matrix m1, m2, p1, p2;
	m1.r = n; m1.c = n; m2.r = n; m2.c = n;

	generate(&m1);
	generate(&m2);

	/*print(m1);
	printf("\n");
	print(m2);
	printf("\n");*/

	p1 = three_nested_loop(m1, m2);
	print(p1);
	printf("\n");

	p2 = bloqued_version(m1, m2, blk);
	print(p2);

	//valgrind and KCachegrind
	//sudo apt-get install kubuntu-desktop

	return 0;
}