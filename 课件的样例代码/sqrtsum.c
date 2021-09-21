#include <stdio.h>
#include <mpi.h>
#include <math.h>
#define N 1002
int main(int argc, char** argv)
{
    int myid, numprocs, source, C=0;
    double data[N], d=0.0, SqrtSum=0.0;

    for (int i = 0; i < N; ++i)
		data[i] = i*(i+1);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

    --numprocs; 
    if (myid == 0) 
    { 
        for (int i = 0; i < N; ++i) 
            MPI_Send(&data[i], 1, MPI_DOUBLE, i%numprocs+1, 1, MPI_COMM_WORLD);

        for (source = 1; source <= numprocs; ++source) 
        {
            MPI_Recv(&d, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status);
            SqrtSum += d;
        }
    } 
    else
    {
        for (int i = myid;i <= N; i+=numprocs) 
        {
            MPI_Recv(&d, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
            SqrtSum+=sqrt(d);
            ++C;
        }
        MPI_Send(&SqrtSum, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD); 
    }
    printf("I am process %d. I recv total %d from process 0, and SqrtSum=%f.\n", myid, C, SqrtSum);
    MPI_Finalize();
}





