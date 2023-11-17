
sort: sort.cxx
	hipcc -D__HIP_PLATFORM_AMD__ -o sort sort.cxx

eig: main.c
	hipcc -D__HIP_PLATFORM_AMD__ -lrocsolver  -lrocblas -o main main.c

