# A playground for symmetric eigensolvers


Currently, the repo contains a single benchmark for the `syevd` eigensolver.

## Prerequisites
```
module load rocm
module load sqlite
```

## Build
```
make
```

## Run
```
./eig <matrix size>
```

## Profile
```
make prof N=<matrix size>
```

## Example
```
make prof N=4096
```
```
calls  grid       kernel                            sec       RDGBs    WRGBs    GBs
-----  ---------  --------------------------------  --------  -------  -------  -------
1      4096       stedc_kernel                      2.99e+02     0.32     0.01     0.33
1      1          stedc_sort                        5.19e+00     0.04     0.05     0.09
8190   599040000  rocblas_symv_kernel_upper_double  2.74e-01   308.63     0.13   308.75
15624  135407616  rocblas_gemvn_kernel              1.34e-01     7.06     0.00     7.06
11842  64995840   rocblas_gemvt_kernel              1.08e-01     8.90     0.00     8.91
8126   17797376   rocblas_scal_kernel               3.57e-02     0.19     0.00     0.19
4094   4192256    rocblas_dot_kernel_magsq          2.79e-02     0.23     0.01     0.24
4095   4193280    rocblas_dot_kernel_inc1by2        2.57e-02     0.22     0.00     0.22
127    4161536    Cijk_Alik_Bljk_DB_MT64x16x16_MI1  1.91e-02   224.84     6.18   231.01
4094   4094       set_taubeta                       1.82e-02     0.21     0.00     0.21
4095   1048320    set_offdiag                       1.62e-02     0.23     0.00     0.23
4031   4127744    rocblas_trmvn_kernel              1.56e-02     0.14     0.00     0.14
43     6111232    DGEMM_Aldebaran_NN_MT128x128x16_  1.51e-02   292.37   194.61   486.98
4095   8912640    scale_axpy                        1.47e-02     0.29     0.00     0.29
4031   2063872    rocblas_copy_kernel               1.38e-02     0.20     0.00     0.20
962    29900800   Cijk_Ailk_Bjlk_DB_MT64x32x8_MI16  9.31e-03    92.94    33.35   126.30
214    15941632   Cijk_Ailk_Bjlk_DB_MT64x64x16_MI1  8.31e-03   199.41   160.02   359.43
928    7495552    Cijk_Ailk_Bjlk_DB_MT16x16x8_MI16  7.14e-03     8.51     0.01     8.52
26     2965504    Cijk_Ailk_Bjlk_DB_MT128x64x8_MI1  3.86e-03   189.07   193.86   382.93
126    719974400  rocblas_syr2k_scale_kernel        3.63e-03     0.03     0.00     0.03
9      2950144    Cijk_Ailk_Bljk_DB_MT128x96x16_MI  3.38e-03   348.51   304.01   652.52
256    33554432   rocblas_trmm_lNx_kernel           2.74e-03    21.62    17.09    38.71
126    16644096   rocblas_syr2k_her2k_kernel        2.17e-03    10.75     0.00    10.76
1      1          stedc_split                       1.27e-03     0.05     0.00     0.05
128    16777216   rocblas_trmm_lTx_kernel           1.27e-03     2.31     2.24     4.55
64     16777216   addmatA1                          1.22e-03   101.40    23.67   125.07
8      606208     Cijk_Ailk_Bjlk_DB_MT128x96x8_MI1  1.14e-03   196.21   200.75   396.97
64     16777216   copymatA1                         1.03e-03   124.72     7.00   131.72
130    4390912    Cijk_Ailk_Bljk_DB_MT32x16x16_MI1  8.45e-04    69.06    32.53   101.60
12     532480     Cijk_Ailk_Bjlk_DB_MT128x96x16_MI  7.92e-04   245.81   238.19   484.00
4      360448     Cijk_Ailk_Bljk_DB_MT128x64x8_MI1  5.78e-04   143.85   138.38   282.22
12     655360     Cijk_Ailk_Bjlk_DB_MT64x128x16_MI  5.21e-04   308.21   211.69   519.90
16     442368     Cijk_Ailk_Bjlk_DB_MT128x64x16_MI  5.19e-04   202.34   192.93   395.27
191    24320      set_tau                           5.16e-04     0.21     0.01     0.22
2      33554432   copy_mat                          4.49e-04   570.11   565.21  1135.32
64     262144     set_triangular                    4.07e-04     2.95     1.43     4.38
6      1081344    Cijk_Ailk_Bljk_DB_MT64x32x8_MI16  3.92e-04   149.82   153.41   303.23
63     286720     rocblas_syr2_kernel               3.30e-04     0.23     0.00     0.23
2      33554432   init_ident                        1.93e-04     0.01  1308.19  1308.21
2      4352       set_tridiag                       1.30e-05    22.08     0.00    22.08
2      512        reset_info                        5.76e-06     0.13     0.00     0.13
1      32         iota_n                            5.44e-06     0.08     0.01     0.08
1      1          reset_batch_info                  3.52e-06     0.24     0.00     0.24
```
