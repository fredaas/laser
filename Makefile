CC=gcc
CFLAGS=-std=c99 -Wall -lm

.PHONY : clean

all : laser_serial laser_omp

laser_serial : laser_serial.c laser.c
	$(CC) -o $@ $^ $(CFLAGS)

laser_omp : laser_omp.c laser.c
	$(CC) -o $@ $^ $(CFLAGS) -fopenmp

run_serial :
	./laser_serial core_gene_alignment.aln distance_matrix.txt

run_omp :
	./laser_omp core_gene_alignment.aln distance_matrix.txt 4

clean :
	rm -f laser laser_serial laser_omp *.o
