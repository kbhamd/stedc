# A playground for symmetric eigensolvers

Currently, the repo contains a single benchmark for the `syevd` eigensolver as provided by hipsolver,
cusolverDn, or rocsolver. The different implementations are acessible through differetn Makefile targets

## Prerequisites
```
module load rocm|cuda
module load sqlite
```

## Build
```
make hip|cuda|roc
```

## Run
```
./eig <matrix size>
```

## Profile
```
make prof|nvprof N=<matrix size>
```

## Example
```
make prof N=4096
```
```
calls  grid       kernel                            sec       RDGBs    WRGBs    GBs      vGFLOPs  mGFLOPs
-----  ---------  --------------------------------  --------  -------  -------  -------  -------  --------
1      4096       stedc_kernel                      3.25e+02     0.30     0.01     0.31     0.50      0.00
1      1          stedc_sort                        5.25e+00     0.04     0.05     0.09     0.00      0.00
8190   599040000  rocblas_symv_kernel_upper_double  2.73e-01   309.92     0.09   310.01   236.67      0.00
15624  135407616  rocblas_gemvn_kernel              1.32e-01     7.18     0.00     7.18    36.01      0.00
11842  64995840   rocblas_gemvt_kernel              1.07e-01     8.99     0.00     8.99    11.64      0.00
8126   17797376   rocblas_scal_kernel               3.48e-02     0.20     0.00     0.20     0.49      0.00
4094   4192256    rocblas_dot_kernel_magsq          2.74e-02     0.23     0.01     0.24     1.58      0.00
4095   4193280    rocblas_dot_kernel_inc1by2        2.50e-02     0.23     0.00     0.23     1.73      0.00
127    4161536    Cijk_Alik_Bljk_DB_MT64x16x16_MI1  1.91e-02   224.83     5.86   230.69     5.24   3603.30
4094   4094       set_taubeta                       1.72e-02     0.20     0.00     0.20     0.73      0.00
4031   4127744    rocblas_trmvn_kernel              1.66e-02     0.16     0.11     0.27     1.22      0.00
4095   1048320    set_offdiag                       1.54e-02     0.24     0.00     0.24     0.00      0.00
43     6111232    DGEMM_Aldebaran_NN_MT128x128x16_  1.52e-02   294.06   193.10   487.16    73.89  12195.50
4095   8912640    scale_axpy                        1.42e-02     0.31     0.00     0.31     2.40      0.00
4031   2063872    rocblas_copy_kernel               1.37e-02     0.20     0.00     0.21     0.00      0.00
962    29900800   Cijk_Ailk_Bjlk_DB_MT64x32x8_MI16  9.31e-03    92.99    24.07   117.06    77.11   1645.04
214    15941632   Cijk_Ailk_Bjlk_DB_MT64x64x16_MI1  8.10e-03   204.59   152.63   357.22    94.42   2014.38
928    7495552    Cijk_Ailk_Bjlk_DB_MT16x16x8_MI16  6.87e-03     8.91     0.02     8.92    13.10    279.43
26     2965504    Cijk_Ailk_Bjlk_DB_MT128x64x8_MI1  3.84e-03   190.00   194.82   384.82    74.05   1579.69
126    719974400  rocblas_syr2k_scale_kernel        3.63e-03     0.03     0.00     0.03     0.00      0.00
9      2950144    Cijk_Ailk_Bljk_DB_MT128x96x16_MI  3.35e-03   351.71   306.40   658.11   126.83   5411.28
256    33554432   rocblas_trmm_lNx_kernel           2.70e-03    21.93    22.09    44.02   806.74      0.00
126    16644096   rocblas_syr2k_her2k_kernel        2.16e-03    10.81     0.00    10.81   509.59      0.00
128    16777216   rocblas_trmm_lTx_kernel           1.42e-03    12.81     2.20    15.01   766.93      0.00
1      1          stedc_split                       1.28e-03     0.05     0.00     0.05     7.79      0.00
64     16777216   addmatA1                          1.22e-03   100.75    27.65   128.41    13.71      0.00
8      606208     Cijk_Ailk_Bjlk_DB_MT128x96x8_MI1  1.14e-03   195.16   200.44   395.61    76.39   1629.68
64     16777216   copymatA1                         1.02e-03   125.73     7.11   132.83     0.00      0.00
130    4390912    Cijk_Ailk_Bljk_DB_MT32x16x16_MI1  8.89e-04    65.80    30.52    96.31    59.26   1320.85
12     532480     Cijk_Ailk_Bjlk_DB_MT128x96x16_MI  7.55e-04   258.01   249.33   507.34   101.53   2166.01
4      360448     Cijk_Ailk_Bljk_DB_MT128x64x8_MI1  5.73e-04   144.49   131.17   275.66    60.36   2575.34
12     655360     Cijk_Ailk_Bjlk_DB_MT64x128x16_MI  5.27e-04   304.76   209.31   514.07   119.48   2548.94
191    24320      set_tau                           5.26e-04     0.23     0.01     0.23     0.00      0.00
16     442368     Cijk_Ailk_Bjlk_DB_MT128x64x16_MI  5.19e-04   203.45   189.84   393.29    81.77   1744.39
2      33554432   copy_mat                          4.36e-04   586.41   577.13  1163.54     0.00      0.00
6      1081344    Cijk_Ailk_Bljk_DB_MT64x32x8_MI16  3.86e-04   152.77   129.58   282.36    67.30   2871.60
64     262144     set_triangular                    3.77e-04     3.17     0.85     4.02     0.52      0.00
63     286720     rocblas_syr2_kernel               3.14e-04     0.24     0.00     0.24     2.47      0.00
2      33554432   init_ident                        1.92e-04     0.02  1309.29  1309.31     0.00      0.00
2      4352       set_tridiag                       1.18e-05    24.17     0.00    24.17     0.00      0.00
1      32         iota_n                            5.60e-06     0.08     0.01     0.08     0.01      0.00
2      512        reset_info                        5.60e-06     0.13     0.00     0.13     0.00      0.00
1      1          reset_batch_info                  3.20e-06     0.27     0.00     0.27     0.00      0.00
```

## Benchmarking
The following results are obtained with the cuda version on A100 and the rocsolver version on MI210
```
---------------------
 size A100   MI210
---------------------
 1024 11.81     3.92
 2048 11.90    33.11
 4096 12.24   368.74
 8192 14.31  3070.6
16384 25.34 21343.
```
