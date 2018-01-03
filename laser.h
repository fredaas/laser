#ifndef LASER_H
#define LASER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/time.h>

void show_sequence(void);
void show_matrix(void);
void show_stats(void);
void map(char symbol);
void read_sequences(FILE *f);
void write_matrix(FILE *f);
void free_memory(void);

double walltime(void);

int compare(char *a, char *b);

#define index(i, j) ((i) * N + (j))

#endif /* LASER_H */
