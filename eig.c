
#include<stdlib.h>
#include<stdio.h>
#include<time.h>


#if defined(cuda)
    #include "cusolverDn.h"
    #define hipFree cudaFree
    #define hipMalloc cudaMalloc
    #define hipMemcpy cudaMemcpy
    #define hipMemcpyHostToDevice cudaMemcpyHostToDevice
    #define hipMemcpyDeviceToHost cudaMemcpyDeviceToHost
    #define hipsolverDnHandle_t cusolverDnHandle_t
    #define hipsolverDnCreate cusolverDnCreate
    #define hipsolverDnDestroy cusolverDnDestroy
    #define hipsolverDnDsyevd_bufferSize cusolverDnDsyevd_bufferSize
    #define hipsolverDnDsyevd cusolverDnDsyevd
    #define HIPSOLVER_EIG_MODE_VECTOR CUSOLVER_EIG_MODE_VECTOR
    #define HIPSOLVER_FILL_MODE_UPPER CUBLAS_FILL_MODE_UPPER
#elif defined(roc)
    #include "hip/hip_runtime.h"
    #include "rocsolver/rocsolver.h"
    #define hipsolverDnHandle_t rocblas_handle
    #define hipsolverDnCreate rocblas_create_handle
    #define hipsolverDnDestroy rocblas_destroy_handle
    #define hipsolverDnDsyevd rocsolver_dsyevd
    #define HIPSOLVER_EIG_MODE_VECTOR rocblas_evect_original
    #define HIPSOLVER_FILL_MODE_UPPER rocblas_fill_upper
#else
    #include "hipsolver/hipsolver.h"
#endif



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

    hipsolverDnHandle_t handle;
    hipsolverDnCreate(&handle);

    struct timespec init;
    clock_gettime(CLOCK_MONOTONIC,&init);
    printf("initme %8.4e\n",clock_delta(&init,&zero));

    int info=-1;
    int *INFO;
    int WORK;

    double *a;
    double *d;
    double *A;
    double *D;
    double *W;
    a=malloc(n*n*sizeof(double));
    d=malloc(n*sizeof(double));

    //Calculate size of work buffer and allocate it
#if !defined(roc)
    hipsolverDnDsyevd_bufferSize(
        handle,                      // rocblas handle                                -> rocblasHandle_t
        HIPSOLVER_EIG_MODE_VECTOR,   // whether or not to  compute eigenvectors       -> hipsolverEigMode_t
        HIPSOLVER_FILL_MODE_UPPER,   // whether upper or lower part of matrix is used -> hipsolverFillMode_t
        n,                           // matrix size                                   -> int
        NULL,                        // real symmetric input matrix (not required)    -> pointer to double
        n,                           // leading dimension of the matrix A             -> int
        NULL,                        // eigenvalues of the matrix A (not required)    -> pointer to double
        &WORK                        // size of work buffer  (output)                 -> pointer to int
    );
#else
    WORK=n;
#endif
    hipMalloc((void **)&W,WORK*sizeof(double));

    hipMalloc((void **)&A,n*n*sizeof(double));
    hipMalloc((void **)&D,n*sizeof(double));
    hipMalloc((void **)&INFO,sizeof(int));

    double MAX=RAND_MAX-1.0;
    for(int i=0;i<n*n;i++)
    {
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

    struct timespec copy2D;
    clock_gettime(CLOCK_MONOTONIC,&copy2D);
    printf("copy2D %8.4e\n",clock_delta(&copy2D, &memalloc));

    int status;
    status=hipsolverDnDsyevd(
        handle,                      // rocblas handle                                -> rocblasHandle_t
        HIPSOLVER_EIG_MODE_VECTOR,   // whether or not to  compute eigenvectors       -> hipsolverEigMode_t
        HIPSOLVER_FILL_MODE_UPPER,   // whether upper or lower part of matrix is used -> hipsolverFillMode_t
        n,                           // matrix size                                   -> int
        A,                           // real symmetric input matrix                   -> pointer to double
        n,                           // leading dimension of the matrix A             -> int
        D,                           // eigenvalues of the matrix A                   -> pointer to double
        W,                           // workspace array                               -> pointer to double
#if !defined(roc)
        WORK,                        // size of work buffer                           -> pointer to int
#endif
        INFO                         // error code
    );

/*
    rocsolver_dsyevd(
        handle,                      // rocblas handle
        rocblas_evect_original,      // whether or not to compute eigenvectors -> rocblas_evect_original
        rocblas_fill_upper,          // whether or upper or lower part of matrix is used -> rocblas_fill
        n,                           // matrix size ,i.e numer of rows/columns of matrix A -> rocblas_int
        A,                           // real symmetric input matrix -> converted to eignevector matrix at exit
        n,                           // leading dimension of matrix A -> rocblas_int
        D,                           // eigenvalues of the matrix A -> pointer to double
        E,                           // workspace array of dimension n -> pointer to double
        INFO                         // error code 0=success -> pointer to rocblas_int
    );
*/

    struct timespec dsyevd;
    clock_gettime(CLOCK_MONOTONIC,&dsyevd);
    printf("dsyevd %8.4e\n",clock_delta(&dsyevd, &copy2D));

    hipMemcpy((void*)d,(void*)D,n*sizeof(double), hipMemcpyDeviceToHost);
    hipMemcpy((void*)&info,(void*)INFO,sizeof(int), hipMemcpyDeviceToHost);

    struct timespec copy2H;
    clock_gettime(CLOCK_MONOTONIC,&copy2H);
    printf("copy2H %8.4e\n",clock_delta(&copy2H,&dsyevd));

    hipFree(A);
    hipFree(D);
    hipFree(W);

    hipsolverDnDestroy(handle);

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

    free(a);
    free(d);
}
