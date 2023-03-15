#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "questions.h"

// Generates question set using recursion
question *create_qset(int *topics, int q_num)
{
    // Break case
    if (q_num < 1)
    {
        return NULL;
    }

    // Allocate space for question
    question *q = malloc(sizeof(question));
    
    if (q == NULL)
    {
        return NULL;
    }

    // Gets te next question
    question *next = create_qset(topics, (q_num -1));

    // Error handling for malloc
    if (next == NULL && q_num != 1)
    {
        free(q);

        return NULL;
    }

    // Generate prompts and answers
    /*
    .
    .
    .
    */

    // Set the next question
    q->next = next;

    return q;
}

// Frees question set using recursion
void free_qset(question *qset, int q_num)
{

}