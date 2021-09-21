#define N 100000000
#define a 0
#define b 10
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int myid, numprocs;
	int i;
	double local = 0.0, dx = (double)(b - a) / N;
	double inte, x;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	for (i = myid; i < N; i = i + numprocs)
	{
		x = a + i * dx + dx / 2;
		local += x * x * dx;
	}
	MPI_Reduce(&local, &inte, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0)
	{
		printf("The integal of x*x in region [%d,%d] =%16.15f\n", a, b, inte);
	}
	MPI_Finalize();
	return 0;
}