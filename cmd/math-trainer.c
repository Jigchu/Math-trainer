#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Macro for array size
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

int main(int argc, char **argv)
{
    // Make getopt not write anything to stderr
    opterr = 0;

    // Opening options file
    FILE *options = fopen("config/options.txt", "r");
    
    if (options == NULL)
    {
        printf("Could not open options.txt\n");
        printf("Perhaps you misplaced it? (It should be in the config folder)\n");

        exit(1);
    }

    // Gets short options
    fseek(options, 0, SEEK_END);
    int len = ftell(options);
    fseek(options, 0, SEEK_SET);

    char *optstr = malloc(sizeof(char) * (len + 1));

    if (optstr == NULL)
    {
        printf("Could not allocate memory for options\n");

        fclose(options);

        exit(2);
    }

    fread(optstr, sizeof(char), len, options);
    optstr[len] = '\0';

    // Long options
    static struct option longopts[] = {
        {"archive", no_argument, NULL, 257},
        {"remember", no_argument, NULL, 258},
    };

    int optlen = len + ARRAY_SIZE(longopts);
    int long_index = -1;

    // Input parsing 

    // Getting the options
    int *opts = malloc((optlen + 1) * sizeof(int));                 /*Stores all the options in argv*/
    int i = 0;                                                      /*Iterable variable*/
    
    while (true)
    {
        opts[i] = getopt_long(argc, argv, optstr, longopts, &long_index);

        // Checks for invalid option
        if (opts[i] == '?')
        {
            printf("Invalid option\n");

            free(optstr);
            free(opts);
            fclose(options);

            exit(3);
        }

        // Checks for the end of options
        if (opts[i] == -1)
        {
            break;
        }    
        
        i++;
    }

    // Checks if no options are inputted
    if (i == 0)
    {
        printf("No options inputted\n");
        
        free(optstr);
        free(opts);
        fclose(options);

        exit(4);
    }

    // Getting all other values

    // Prevent integer overflow
    if (atoll(argv[optind]) > INT_MAX)
    {
        printf("Integer overflow: please input something less or equal to %d\n", INT_MAX);
        
        free(optstr);
        free(opts);
        fclose(options);

        exit(5);
    }

    int q_num = atoi(argv[optind]);

    // The equation generation part
    /*
    .
    .
    .
    */

    // Archival system

    // Frees all memory
    free(optstr);
    free(opts);
    fclose(options);

    exit(EXIT_SUCCESS);
}
