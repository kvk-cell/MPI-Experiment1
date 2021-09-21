#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
int main(int argc, char **argv)
{
    int myid, numprocs, N;
    int *data;
    double local=0.0, SqrtSum=0.0;
    double startwtime = 0.0, endwtime;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    

    if(myid == 0)
    {
        printf("Input the size of the array:");
        fflush(stdout);
        scanf("%d", &N);
        startwtime = MPI_Wtime();
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    data = (int *)malloc(sizeof(int)*N);
    for (int i = 0; i < N; ++i)
        data[i] = i * (i + 1);

    for (int i = myid; i < N; i += numprocs)
    {
        local += sqrt(data[i]);
    }

    MPI_Reduce(&local, &SqrtSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myid == 0)
    {
        printf("SqrtSum=%f.\n", SqrtSum);
        fflush(stdout);

        endwtime = MPI_Wtime();
		printf("wall clock time = %f\n", endwtime - startwtime);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}