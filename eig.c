#include "hip/hip_runtime_api.h"
#include "rocsolver/rocsolver.h"
#include<stdlib.h>
#include<stdio.h>
#include<time.h>


float clock_delta(struct timespec *y, struct timespec *x){return y->tv_sec-x->tv_sec +1e-9*(y->tv_nsec -x->tv_nsec);}

int main(int argc, const char **argv)
{
    struct timespec zero;
    clock_gettime(CLOCK_MONOTONIC,&zero);

    int n=0;
    if(argc==1)
    {
        printf("no matrix size specified\n");
        exit(0);
    }
    else
    {
        n=atoi(argv[1]);
        printf("requested matrix size %d\n\n",n);
    }

    rocblas_handle handle;
    rocblas_create_handle(&handle);
    rocblas_initialize();

    struct timespec init;
    clock_gettime(CLOCK_MONOTONIC,&init);
    printf("initme %8.4e\n",clock_delta(&init,&zero));

    int sweeps=-1; //sweeps syevj only
    double norm=-1.0; //error  syevj only
    double tol=-1.0;
    int info=-1;
    int *INFO;
    int *SWEEPS;
    double *NORM;
    double *TOL;

    double *a;
    double *d;
    double *e;
    double *A;
    double *D;
    double *E;
    a=malloc(n*n*sizeof(double));
    d=malloc(n*sizeof(double));
    e=malloc(n*sizeof(double));

    hipMalloc((void **)&A,n*n*sizeof(double));
    hipMalloc((void **)&D,n*sizeof(double));
    hipMalloc((void **)&E,n*sizeof(double));
    hipMalloc((void **)&INFO,sizeof(int));
    hipMalloc((void **)&SWEEPS,sizeof(int));
    hipMalloc((void **)&TOL,sizeof(double));
    hipMalloc((void **)&NORM,sizeof(double));

    double MAX=RAND_MAX-1.0;
    for(int i=0;i<n*n;i++)
    {
        //a[i]=1.0;
        a[i] = rand()/MAX;
    }
    for(int i=0;i<n;i++)
    {
        d[i]=-1.0;
    }
    size_t nbytes=n*n*sizeof(double);

    struct timespec memalloc;
    clock_gettime(CLOCK_MONOTONIC,&memalloc);
    printf("malloc %8.4e\n",clock_delta(&memalloc,&init));

    hipMemcpy((void*)A,(void*)a,nbytes, hipMemcpyHostToDevice);
    hipMemcpy((void*)D,(void*)d,n*sizeof(double), hipMemcpyHostToDevice);
    hipMemcpy((void*)TOL,(void*)&tol,sizeof(double), hipMemcpyHostToDevice);

    struct timespec copy2D;
    clock_gettime(CLOCK_MONOTONIC,&copy2D);
    printf("copy2D %8.4e\n",clock_delta(&copy2D, &memalloc));

    rocsolver_dsyevd(
        handle,                          // rocblas handle
        rocblas_evect_original,          // whether or not to compute eigenvectors -> rocblas_evect_original
        rocblas_fill_upper,              // whether or upper or lower part of matrix is used -> rocblas_fill
        n,                               // matrix size ,i.e numer of rows/columns of matrix A -> rocblas_int
        A,                               // real symmetric input matrix -> converted to eignevector matrix at exit
        n,                               // leading dimension of matrix A -> rocblas_int
        D,                               // eigenvalues of the matrix A -> pointer to double
        E,                               // workspace array of dimension n -> pointer to double
        INFO                             // error code 0=success -> pointer to rocblas_int
    );
    /*
    rocsolver_dsyevj(
        handle,                          //rocblas handle
        rocblas_esort_none,              //rocblas_esort_ascending || rocblas_sort_none
        rocblas_evect_original,          //rocblas_evect_original
        rocblas_fill_upper,              //rocblas_fill_upper
        n,                               //matrix dimension
        A,                               //matrix
        n,                               //leading matrix dimension == n
        tol,                             //convergence tolerance -1 implies machine epsilon
        NORM,                            //error norm -> pointer to double
        1024,                            //max_sweeps -> maximum number of sweeps
        SWEEPS,                          //sweeps -> pointer to int
        D,                               //eigenvalues -> pointer to double of dimension n
        INFO                             //exit status -> 0 indicates success
    );
    */

    struct timespec dsyevd;
    clock_gettime(CLOCK_MONOTONIC,&dsyevd);
    printf("dsyevd %8.4e\n",clock_delta(&dsyevd, &copy2D));

    hipMemcpy((void*)d,(void*)D,n*sizeof(double), hipMemcpyDeviceToHost);
    hipMemcpy((void*)&info,(void*)INFO,sizeof(int), hipMemcpyDeviceToHost);
    hipMemcpy((void*)&sweeps,(void*)SWEEPS,sizeof(int), hipMemcpyDeviceToHost);
    hipMemcpy((void*)&norm,(void*)NORM,sizeof(double), hipMemcpyDeviceToHost);

    struct timespec copy2H;
    clock_gettime(CLOCK_MONOTONIC,&copy2H);
    printf("copy2H %8.4e\n",clock_delta(&copy2H,&dsyevd));

    free(a);
    free(d);
    free(e);
    hipFree(A);
    hipFree(D);
    hipFree(E);

    rocblas_destroy_handle(handle);

    struct timespec memfree;
    clock_gettime(CLOCK_MONOTONIC,&memfree);
    printf("freeme %8.4e\n",clock_delta(&memfree, &copy2H));

    printf("total  %8.4e\n",clock_delta(&memfree,&zero));

    printf("\n");
    for(int i=0;i<8;i++)
    {
        printf("%f\n",d[i]);
    }
    printf("%f\n",d[n-1]);

    printf("info=%d\n",info);
}
