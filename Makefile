
nums = 1 2 3 4 5 6 7
CC = mpicc
RR = mpirun
RFLAGS = -np
CFLAGS = 
SRC = ./src

build : ${SRC}/utils.c ${SRC}/utils.h
	lamboot
	for i in ${nums} ; do \
	${CC} ${CFLAGS} -o mpiCode$$i ${SRC}/mpiCode$$i.c ${SRC}/utils.c ; \
	done \

clean: 
	rm -rf *.o a.out mpiCode*

# sorting command of output.txt 
# sort --numeric-sort --stable output.txt