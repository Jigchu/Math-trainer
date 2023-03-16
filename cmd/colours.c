#include <stdio.h>

#include "colours.h"

void red(void)
{
    fprintf(stdout, "\033[38;2;255;0;0m");
    return;
}
void blue(void)
{
    fprintf(stdout, "\033[38;2;0;255;255m");
    return;
}
void green(void)
{
    fprintf(stdout, "\033[38;2;0;210;0m");
    return;
}
void reset(void)
{
    fprintf(stdout, "\033[0m");
    return;
}