#include <ctype.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "questions.h"

// Macro for array size
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

// Function Prototypes (These are usually for input parsing)
bool has_char(char *word);

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

    if (len == 0)
    {
        fprintf(stderr, "There are no options in config/options.txt\n");
        exit(2);
    }

    char *optstr = malloc(sizeof(char) * (len + 1));

    if (optstr == NULL)
    {
        fprintf(stderr, "Could not allocate memory for options\n");

        fclose(options);

        exit(3);
    }

    fread(optstr, sizeof(char), len, options);
    optstr[len] = '\0';

    // Close option file
    fclose(options);

    // Long options
    static struct option longopts[] = {
        {"archive", no_argument, NULL, 257},
        {"remember", no_argument, NULL, 258},
        {0, 0, 0, 0}
    };

    int long_index = -1;

    // Input parsing 

    // Getting the options
    int *opts = malloc(sizeof(int));                 /*Stores all the options in argv*/
    int optlen = 0;                                       /*Iterable variable*/
    
    while (true)
    {
        opts[optlen] = getopt_long(argc, argv, optstr, longopts, &long_index);

        // Checks for invalid option
        if (opts[optlen] == '?')
        {
            printf("Invalid option %s\n", argv[optind]);

            free(optstr);
            free(opts);

            exit(4);
        }

        // Checks for the end of options
        if (opts[optlen] == -1)
        {
            break;
        }    
        
        // Allocates more memory with realloc
        optlen++;

        int *temp = realloc(opts, (optlen + 1) * sizeof(int));

        if (temp == NULL)
        {
            fprintf(stderr, "Could not reallocate memory\n");

            free(opts);
            free(optstr);

            exit(5);
        }

        opts = temp;
    }

    // Frees optstr
    free(optstr);

    // Checks if no options are inputted
    if (optlen == 0)
    {
        fprintf(stderr, "No options inputted\n");
        
        free(opts);

        exit(6);
    }

    // Getting all other values

    // Checks if the number has a char
    if (has_char(argv[optind]))
    {
        fprintf(stderr, "%s has a character\n", argv[optind]);

        free(opts);

        exit(7);
    }
    

    // Prevent integer overflow
    if (atoll(argv[optind]) > INT_MAX)
    {
        fprintf(stderr, "Integer overflow: %s is larger than %d\n", argv[optind], INT_MAX);

        free(opts);

        exit(8);
    }

    int q_num = atoi(argv[optind]);

    // Generates the question set
    int *topics = NULL;

    //question *qset = create_qset(topics, q_num);

    // Frees all memory
    free(opts);
    free(topics);

    exit(EXIT_SUCCESS);
}


// Checks if a string has a char
bool has_char(char *word)
{
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        if (!isdigit(word[i]))
        {
            return true;
        }
    }

    return false;
}