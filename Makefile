

eig: eig.c
	hipcc -D__HIP_PLATFORM_AMD__ -lrocsolver  -lrocblas -o eig eig.c

sort: sort.cxx
	hipcc -D__HIP_PLATFORM_AMD__ -o sort sort.cxx

prof: eig
	./eig
