#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 200

unsigned max_age;
char *input_file = NULL;
char *output_file = NULL;

void filter_stream(FILE *in, FILE *out) {
    char line[MAXLINE];
    char copy[MAXLINE];
    unsigned age;
    int line_no = 0;

    while (fgets(line, MAXLINE, in) != NULL) {
        line_no++;

        /* Check for empty or newline-only lines */
        if (line[0] == '\n' || strlen(line) < 2) {
            fprintf(stderr, "Line %d: Whole line missing.\n", line_no);
            continue;
        }

        /* Make a copy because strtok modifies the input */
        strcpy(copy, line);

        /* Split on comma */
        char *name = strtok(copy, ",");
        char *age_str = strtok(NULL, ",");

        if (name == NULL || strlen(name) == 0) {
            fprintf(stderr, "Line %d: Whole line missing.\n", line_no);
            continue;
        }

        if (age_str == NULL) {
            fprintf(stderr, "Line %d: Age missing.\n", line_no);
            continue;
        }

        /* Try converting the age */
        if (sscanf(age_str, "%u", &age) != 1) {
            fprintf(stderr, "Line %d: Age not recognized.\n", line_no);
            continue;
        }

        /* Check the age */
        if (age <= max_age) {
            /* Write the original line to output */
            fputs(line, out);
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *in = NULL;
    FILE *out = NULL;

    if (argc < 2) {
        printf("Usage: %s max-age [input-file] [output-file]\n", argv[0]);
        return 0;
    }

    if (sscanf(argv[1], "%u", &max_age) != 1) {
        printf("First argument is not an age.\n");
        return 1;
    }

    if (argc >= 3) {
        input_file = argv[2];
    }

    if (argc >= 4) {
        output_file = argv[3];
    }

    /* Open input */
    if (input_file != NULL) {
        in = fopen(input_file, "r");
        if (in == NULL) {
            printf("Cannot open input file '%s'.\n", input_file);
            return 1;
        }
    } else {
        in = stdin;
    }

    /* Open output */
    if (output_file != NULL) {
        out = fopen(output_file, "w");
        if (out == NULL) {
            printf("Cannot open output file '%s'.\n", output_file);
            return 1;
        }
    } else {
        out = stdout;
    }

    filter_stream(in, out);

    return 0;
}
