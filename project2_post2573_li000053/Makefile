C=gcc
CFLAGS=-pipe

all: sim_multi.o
	$(CC) $(CFLAGS) sim_multi.o -o sim_multi

myarr.o: sim_multi.c
	$(CC) $(CFLAGS) -c sim_multi.c

clean: 
	rm sim_multi *.o *.*~ *~ 
