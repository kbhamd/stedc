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

