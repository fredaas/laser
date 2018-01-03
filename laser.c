#include "laser.h"

const char DNASEQ[] = "ACGT-N";

int stats[6] = { 0 };

int *matrix;

/* Sequence length. */
int M;
/* Number of sequences. */
int N;

char **sequence;

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

double walltime(void)
{
  static struct timeval t;
  gettimeofday(&t, NULL);

  return t.tv_sec + 1e-6 * t.tv_usec;
}
