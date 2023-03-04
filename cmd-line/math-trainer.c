#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Make getopt not write anything to stderr
int opterr = 0;

int main(int argc, char const *argv[])
{
    // Opening options file
    FILE *options = fopen("options.txt", "r");
    
    if (options == NULL)
    {
        printf("Could not open options.txt\n");
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
        return 2;
    }

    fread(optstr, sizeof(char), len, options);
    optstr[len] = '\0';

    // Long options
    static struct option longopts[] = {

    };

    return 0;
}
