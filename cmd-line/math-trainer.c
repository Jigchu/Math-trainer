#include <getopt.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Macro for array size
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

// Make getopt not write anything to stderr
int opterr = 0;

int main(int argc, char const *argv[])
{
    // Opening options file
    FILE *options = fopen("options.txt", "r");
    
    if (options == NULL)
    {
        printf("Could not open options.txt\n");
        printf("Perhaps you misplaced it? (It should be in the config folder)\n");
        return 1;
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
        return 2;
    }

    fread(optstr, sizeof(char), len, options);
    optstr[len] = '\0';

    // Long options
    static struct option longopts[] = {
        {"archive", no_argument, NULL, 256},
        {"remember", no_argument, NULL, 255},
    };

    // Input parsing
    uint8_t *opts = malloc(len + ARRAY_SIZE(longopts));


    // Frees all memory
    free(optstr);
    fclose(options);

    return 0;
}
