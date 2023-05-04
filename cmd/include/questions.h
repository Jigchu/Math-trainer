// Header file for question manipulation and generation
#pragma once

// Question struct
typedef struct question
{
    char *prompt;
    char *answer;
    struct question *next;
} question;

// Struct when using create_q with multi-threading
typedef struct create_arg
{
    int *topic;
    int q_num;
} create_arg;

// Question set
question *qset = NULL;

// Function prototypes
int create_qset(create_arg args);
void free_qset(void);