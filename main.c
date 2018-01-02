#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void show_sequence(void);
void show_matrix(void);
void show_stats(void);
void generate_matrix(void);
void map(char symbol);
void read_sequences(FILE *f);
void write_matrix(FILE *f);

#define index(i, j) ((i) * N + (j))

const char DNASEQ[] = "ACGT-N";

int stats[6] = { 0 };

int *matrix;

/* Sequence length. */
int M;
/* Number of sequences. */
int N;

char **sequence;

int world_size;
int rank;

void map(char symbol)
{
    switch (symbol) {
        case 'A':
            stats[0]++;
            break;
        case 'C':
            stats[1]++;
            break;
        case 'G':
            stats[2]++;
            break;
        case 'T':
            stats[3]++;
            break;
        case '-':
            stats[4]++;
            break;
        case 'N':
            stats[5]++;
            break;
    }
}

void read_sequences(FILE *f)
{
    int i = 0;
    int buffer_size = 256;

    char f_word[buffer_size];
    char *f_buffer = (char *) malloc(buffer_size * sizeof(char));

    /* Read first sequence index. */
    fscanf(f, "%s", f_word);

    /* Read first sequence and calculate the sequence length. */
    while (fscanf(f, "%s", f_word) != EOF) {
        if (f_word[0] == '>') {
            i++;
            sequence = (char **) realloc(sequence, (i + 1) * sizeof(char *));
            sequence[i] = (char *) malloc(M * sizeof(char));
            break;
        }

        M += strlen(f_word);

        if (M > buffer_size) {
            buffer_size *= 2;
            f_buffer = (char *) realloc(f_buffer, buffer_size * sizeof(char));
        }

        strcat(f_buffer, f_word);
    }

    sequence[0] = (char *) malloc(M * sizeof(char));
    strcpy(sequence[0], f_buffer);

    /* Read the rest of the sequences. */
    while (fscanf(f, "%s", f_word) != EOF) {
        if (f_word[0] == '>') {
            i++;
            sequence = (char **) realloc(sequence, (i + 1) * sizeof(char *));
            sequence[i] = (char *) malloc(M * sizeof(char));
            continue;
        }

        strcat(sequence[i], f_word);
    }

    N = i + 1;

    free(f_buffer);
}

int compare(char *a, char *b)
{
    int popcount = 0;

    for (int i = 0; i < M; i++) {
        if (a[i] == '-') {
            continue;
        }

        if (toupper(a[i]) != toupper(b[i])) {
            popcount++;
        }
    }

    return popcount;
}

void generate_matrix(void)
{
    matrix = (int *) calloc(N * N, sizeof(int));

    for (int i = 0; i < N; i++) {
        char *current = sequence[i];
        for (int j = 0; j < N; j++) {
            char *next = sequence[j];
            matrix[index(i, j)] = compare(current, next);
        }
    }
}

void show_matrix(void)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[index(i, j)]);
        }
        printf("\n");
    }
}

void write_matrix(FILE *f)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(f, "%d ", matrix[index(i, j)]);
        }
        fprintf(f, "\n");
    }
}

void show_sequence(void)
{
    for (int i = 0; i < N; i++) {
        printf("[%d] %s\n", i, sequence[i]);
    }
}

void show_stats(void)
{
    for (int i = 0; i < 6; i++) {
        printf("[%c] %d\n", DNASEQ[i], stats[i]);
    }
}

void free_memory(void)
{
    for (int i = 0; i < N; i++) {
        free(sequence[i]);
    }

    free(sequence);
    free(matrix);
}

int main(int argc, char **argv)
{
    char target_in[256];
    char target_out[256];

    strcpy(target_in, argv[1]);
    strcpy(target_out, argv[2]);

    if (access(target_in, W_OK) == -1) {
        printf("ERROR! Input file does not exist.\n");
    }

    if (access(target_out, W_OK) == -1) {
        printf("ERROR! Output file does not exist.\n");
    }

    FILE *in = fopen(target_in, "r");
    FILE *out = fopen(target_out, "w");

    read_sequences(in);
    generate_matrix();
    write_matrix(out);
    free_memory();

    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
