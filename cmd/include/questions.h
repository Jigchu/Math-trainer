// Header file for question manipulation and generation
#pragma once

// Question struct
typedef struct question
{
    char *prompt;
    char *answer;
    struct question *next;
} question;

// Question set
question *qset = NULL;

// Function prototypes
int create_qset(int *topics, int *q_num);
void free_qset(question *qset);