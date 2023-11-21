

hip: eig.c
	hipcc -D__HIP_PLATFORM_AMD__ -lhipsolver -o eig eig.c

roc: eig.c
	hipcc -D__HIP_PLATFORM_AMD__ -Droc -lrocsolver -lrocblas -o eig eig.c

cuda: eig.c
	nvcc -Dcuda -lcusolver -o eig eig.c


sort: sort.cxx
	hipcc -D__HIP_PLATFORM_AMD__ -o sort sort.cxx

prof: eig
	rocprof --stats --basenames on -i eig.txt ./eig $(N)
	sqlite3 < eig.sql

nvprof: eig
	nsys profile -t cuda -o eig --stats=true --force-overwrite=true ./eig $(N) 1>/dev/null
	sqlite3 < nsys.sql
