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
static int *get_options(int argc, char *const *argv, char *optstr, struct option *longopts, int *option_err);
static bool has_char(char *word);
static int *get_topics(int *opts, int optlen);

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

    char *optstr = malloc((len + 1) * sizeof(char));

    if (optstr == NULL)
    {
        fprintf(stderr, "Could not allocate memory for option string\n");

        fclose(options);

        exit(3);
    }

    fread(optstr, sizeof(char), len, options);
    optstr[len] = '\0';

    // Close option file
    fclose(options);

    // Long options
    static struct option longopts[] = {
        {"archive", no_argument, NULL, 256},
        {"remember", no_argument, NULL, 257},
        {0, 0, 0, 0}
    };

    // Input parsing

    // Gets options
    int option_err = 0;
    int *tmp = get_options(argc, argv, optstr, longopts, &option_err);

    // Frees optstr
    free(optstr);

    // Errors handling for get_options

    // Memory allocation problems
    if (option_err == -1)
    {
        fprintf(stderr, "Could not allocate memory for options\n");

        exit(3);
    }

    if (option_err == -3)
    {
        fprintf(stderr, "Could not reallocate memory\n");

        exit(4);
    }

    // Errors with input
    if (option_err == -2)
    {
        // This is its own var so that the line would not be too long
        char *inv_opt = (!(argc == optind) && (argv[optind][0] == '-' && argv[optind][1] != '-')) ? argv[optind] : argv[optind - 1];
        
        fprintf(stderr, "Invalid option %s\n", inv_opt);

        exit(5);
    }

    if (option_err == -4)
    {
        fprintf(stderr, "No options inputted\n");

        exit(6);
    }

    int *opts = tmp;

    // Getting all other values

    // Checks if the user has included the total question
    if (argc <= optind)
    {
        fprintf(stderr, "Usage: ./math-trainer -[topics] --[extra long options] [total questions]\n");

        free(opts);

        exit(7);
    }

    // Checks if the number has a char
    if (has_char(argv[optind]))
    {
        fprintf(stderr, "%s has a character\n", argv[optind]);

        free(opts);

        exit(8);
    }
    

    // Prevent integer overflow
    if (atoll(argv[optind]) > INT_MAX)
    {
        fprintf(stderr, "Integer overflow: %s is larger than %d\n", argv[optind], INT_MAX);

        free(opts);

        exit(9);
    }

    int q_num = atoi(argv[optind]);

    // Generates the question set
    int *topics = get_topics(opts, opts[0]);

    if (topics == NULL)
    {
        fprintf(stderr, "Could not allocate memory for topics\n");

        free(opts);

        exit(10);
    }

    if (topics[0] <= 1)
    {
        fprintf(stderr, "Did not input any topics\n");
        fprintf(stderr, "Usage: ./math-trainer -[topics] --[extra long options] [total questions]\n");

        free(opts);

        exit(11);
    }

    question *qset = create_qset(topics, q_num);

    // Frees all memory
    free(opts);
    free(topics);
    free_qset(qset);

    exit(EXIT_SUCCESS);
}


// Function to parse options
static int *get_options(int argc, char *const *argv, char *optstr, struct option *longopts, int *option_err)
{
    // temporary value for longopt to use
    int tmp = -1;

    // Getting the options
    int *opts = malloc(2 * sizeof(int));

    if (opts == NULL)
    {   
        *option_err = -1;

        return NULL;
    }

    int optlen = 1;                                       /*Iterable variable*/
    
    while (true)
    {   
        opts[optlen] = getopt_long(argc, argv, optstr, longopts, &tmp);

        // Checks for invalid option
        if (opts[optlen] == '?')
        {
            free(opts);

            *option_err = -2;

            return NULL;
        }

        // Checks for the end of options
        if (opts[optlen] == -1)
        {
            break;
        }    
        
        optlen++;

        // Allocates more memory with realloc
        int *temp = realloc(opts, (optlen + 1) * sizeof(int));

        if (temp == NULL)
        {
            free(opts);

            *option_err = -3;

            return NULL;
        }

        opts = temp;
    }

    // Checks if no options are inputted
    if (optlen == 0)
    {
        free(opts);

        *option_err = -4;

        return NULL;
    }

    opts[0] = optlen;

    return opts;
}

// Checks if a string has a char
static bool has_char(char *word)
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

static int *get_topics(int *options, int optlen)
{
    // Pointer to store the topics
    int *topics = malloc(1 * sizeof(int));

    if (topics == NULL)
    {
        return NULL;
    }

    // Record the length
    int len = 1;

    for (int i = 1; i < optlen; i++)
    {
        // Check if it is a shortopt or longopt
        int option = options[i];

        if (option < 256)
        {
            // Reallocate memory with realloc
            int *tmp = realloc(topics, (len + 1) * sizeof(int));

            if (tmp == NULL)
            {
                // Free memory used by function
                if (topics != NULL)
                {
                    free(topics);
                }

                return NULL;
            }

            topics = tmp;

            // Adds topic
            topics[i + 1] = option;

            len++;
        }
    }

    // Store the metadata
    topics[0] = len;

    return topics;
}