
nums = 1 2 3 4 5 6 7
CC = mpicc
RR = mpirun
RFLAGS = -np
CFLAGS = -Wall
SRC = ./src

build :
	lamboot
	for i in ${nums} ; do \
	${CC} ${CFLAGS} -o mpiCode$$i ${SRC}/mpiCode$$i.c; \
	done \

clean: 
	rm -rf *.o a.out mpiCode*

# sorting command of output.txt 
# sort --numeric-sort --stable output.txt