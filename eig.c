#include "hip/hip_runtime_api.h"
#include "rocsolver/rocsolver.h"
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int main()
{

rocblas_handle handle;
rocblas_create_handle(&handle);

clock_t zero=clock();
rocblas_initialize();
clock_t init=clock();

int sweeps=-1; //sweeps syevj only
double norm=-1.0; //error  syevj only
double tol=-1.0;
int info=-1;
int *INFO;
int *SWEEPS;
double *NORM;
double *TOL;

int n= (1<<12);//12800;
printf("n=%d\n",n);

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

float MAX=RAND_MAX;
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


clock_t memalloc=clock();
//hipMemcpy((void*)A,(void*)a,sizeof(double)*n,hipMemcpyHtoD);
hipMemcpy((void*)A,(void*)a,nbytes, hipMemcpyHostToDevice);
hipMemcpy((void*)D,(void*)d,n*sizeof(double), hipMemcpyHostToDevice);
hipMemcpy((void*)TOL,(void*)&tol,sizeof(double), hipMemcpyHostToDevice);

clock_t copy2D=clock();

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
            
clock_t dsyevd=clock();


hipMemcpy((void*)d,(void*)D,n*sizeof(double), hipMemcpyDeviceToHost);
hipMemcpy((void*)&info,(void*)INFO,sizeof(int), hipMemcpyDeviceToHost);
hipMemcpy((void*)&sweeps,(void*)SWEEPS,sizeof(int), hipMemcpyDeviceToHost);
hipMemcpy((void*)&norm,(void*)NORM,sizeof(double), hipMemcpyDeviceToHost);

clock_t copy2H=clock();

for(int i=0;i<8;i++)
{
    printf("%f\n",d[i]);
}
printf("%f\n",d[n-1]);

printf("info=%d\n",info);


printf("total =%f\n",(double)(copy2H-zero)/CLOCKS_PER_SEC);
printf("init  =%f\n",(double)(init-zero)/CLOCKS_PER_SEC);
printf("alloc =%f\n",(double)(memalloc-init)/CLOCKS_PER_SEC);
printf("copy2D=%f\n",(double)(copy2D-memalloc)/CLOCKS_PER_SEC);
printf("syevd =%f\n",(double)(dsyevd-copy2D)/CLOCKS_PER_SEC);
printf("copy2H=%f\n",(double)(copy2H-dsyevd)/CLOCKS_PER_SEC);

free(a);
free(d);
free(e);
hipFree(A);
hipFree(D);
hipFree(E);


printf("info: %d\n",info);
rocblas_destroy_handle(handle);

}
