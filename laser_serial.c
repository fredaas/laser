#include "laser.h"

extern char **sequence;
extern int *matrix;
extern int N;
extern int M;

double start, end;

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

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);

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

    printf("Reading sequences... ");

    start = walltime();
    read_sequences(in);
    end = walltime();

    printf("Done! Time: %.2lf\n", end - start);

    printf("Generating matrix...\n");

    start = walltime();
    generate_matrix();
    end = walltime();

    printf("Done! Time: %.2lf\n", end - start);

    printf("Wrinting result...\n");

    write_matrix(out);

    free_memory();

    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
