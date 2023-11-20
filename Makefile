

eig: eig.c
	hipcc -D__HIP_PLATFORM_AMD__ -lhipsolver -lrocsolver -lrocblas -o eig eig.c

sort: sort.cxx
	hipcc -D__HIP_PLATFORM_AMD__ -o sort sort.cxx

prof: eig
	rocprof --stats --basenames on -i eig.txt ./eig $(N)
	sqlite3 < eig.sql
