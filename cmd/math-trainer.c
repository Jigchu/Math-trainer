#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro for array size
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

int main(int argc, char *const *argv)
{
    // Make getopt not write anything to stderr
    opterr = 0;

    // Opening options file
    FILE *options = fopen("config/options.txt", "r");
    
    if (options == NULL)
    {
        fprintf(stderr, "Could not open options.txt\n");
        fprintf(stderr, "Perhaps you misplaced it? (It should be in the config folder)\n");

        exit(1);
    }

    // Gets short options
    fseek(options, 0, SEEK_END);
    int len = ftell(options);
    fseek(options, 0, SEEK_SET);

    char *optstr = malloc(sizeof(char) * (len + 1));

    if (optstr == NULL)
    {
        fprintf(stderr, "Could not allocate memory for options\n");

        fclose(options);

        exit(2);
    }

    fread(optstr, sizeof(char), len, options);
    optstr[len] = '\0';

    // Close option file
    fclose(options);

    // Long options
    static struct option longopts[] = {
        {"archive", no_argument, NULL, 257},
        {"remember", no_argument, NULL, 258},
    };

    int long_index = -1;

    // Input parsing 

    // Getting the options
    int *opts = malloc(sizeof(int));                 /*Stores all the options in argv*/
    int i = 0;                                       /*Iterable variable*/
    
    while (true)
    {
        opts[i] = getopt_long(argc, argv, optstr, longopts, &long_index);

        // Checks for invalid option
        if (opts[i] == '?')
        {
            printf("Invalid option\n");

            free(optstr);
            free(opts);

            exit(3);
        }

        // Checks for the end of options
        if (opts[i] == -1)
        {
            break;
        }    
        
        // Allocates more memory with realloc
        i++;

        int *temp = realloc(opts, (i + 1) * sizeof(int));

        if (temp == NULL)
        {
            fprintf(stderr, "Could not reallocate memory\n");

            free(opts);
            free(optstr);

            exit(4);
        }

        opts = temp;
    }

    // Frees optstr
    free(optstr);

    // Checks if no options are inputted
    if (i == 0)
    {
        fprintf(stderr, "No options inputted\n");
        
        free(opts);

        exit(5);
    }

    // Getting all other values
    char *tmp = argv[optind];

    // Prevent non numerical characters
    for (int j = 0, len = strlen(tmp); i < len; i++)
    {
        if (!isdigit(tmp[j]))
        {
            fprintf(stderr, "Typed character instead of an integer\n");
            
            free(opts);

            exit(6);
        }
    }

    // Prevent integer overflow
    if (atoll(argv[optind]) > INT_MAX)
    {
        fprintf(stderr, "Integer overflow: please input something less or equal to %d\n", INT_MAX);

        free(opts);

        exit(7);
    }

    int q_num = atoi(argv[optind]);

    // The equation generation part
    /*
    .
    .
    .
    */

    // Frees all memory
    free(optstr);
    free(opts);
    fclose(options);

    exit(EXIT_SUCCESS);
}
