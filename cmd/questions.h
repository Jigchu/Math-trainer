// Header file for question manipulation and generation

#ifndef QUESTIONS_H
#define QUESTIONS_H

// Question struct
typedef struct question
{
    char *prompt;
    char *answer;
    struct question *next;
} question;

// Function prototypes
question *create_qset(int *topics, int q_num);
void free_qset(question *qset, int q_num);

#endif