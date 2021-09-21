#define N 100000000
#define a 10
#define b 100
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char** argv)
{
    int myid, numprocs, source, C = 0;
	double local = 0.0, dx = (double)(b - a) / N;
	double inte, x;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

    --numprocs; 
    if (myid != 0)
    {
        for (int i = myid-1;i < N; i += numprocs) 
        {
            x = a + i * dx + dx / 2;
            inte += x * x * x * dx;
            ++C;
        }
        MPI_Send(&inte, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD); 
    }
    else
    {
        for (source = 1; source <= numprocs; ++source) 
        {
            MPI_Recv(&local, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status);
            inte += local;
        }
    }
    printf("I am process %d, I recv total %d from process 0, and inte=%f.\n", myid, C, inte);

    if (myid == 0)
    {
        printf("The integal of x^3 in region [%d,%d] =%16.15f\n", a, b, inte);
        fflush(stdout);
    }
    
    MPI_Finalize(); 
}