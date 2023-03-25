#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "questions.h"

// Generates question set using iteration so as to save space
question *create_qset(int *topics, int q_num)
{
    // Allocate space for question
    question *head = malloc(sizeof(question));
    
    if (head == NULL)
    {
        return NULL;
    }

    // Setting the head's next
    head->next = NULL;

    // Variable to keep track of the current allocated question
    question *current = head;

    // Generates the rest of the question set
    for (int i = 1; i < q_num; i++)
    {
        // Gets te next question
        question *next = malloc(sizeof(question));
    
        if (next == NULL)
        {
            // Free the question set
            free_qset(head);

            return NULL;
        }

        // Setting the head's next
        next->next = NULL;

        // Generate prompts and answers
        /*
        *.
        *.
        *.
        */

        // Set the next question
        current->next = next;

        // Set the current question
        current = next;
    }

    return head;
}

// Frees question set using recursion
void free_qset(question *qset)
{
    // Break Case
    if (qset == NULL)
    {
        return;
    }

    // Gets the next question
    question *next = qset->next;

    // Free the next question and all the questions following it
    free_qset(next);

    // Free the current question
    free(qset);

    return;
}