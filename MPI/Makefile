
CC = mpicc
RR = mpirun
RFLAGS = -np
CFLAGS = 
PROCS = 9
SIZE = 9 

build:
	lamboot
	${CC} ${CFLAGS} -o matmult matmult.c -lm
	${RR} ${RFLAGS} ${PROCS} ./matmult ${SIZE}

clean: 
	rm -rf matmult
