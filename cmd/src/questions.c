#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "questions.h"
#include "random.h"

// Generates question set using iteration so as to save space
question *create_qset(int *topics, int q_num)
{
    // Allocate space for question
    question *head = malloc(sizeof(question));
    
    if (head == NULL)
    {
        return NULL;
    }

    // Zeroing all values
    head->next = NULL;
    head->answer = NULL;
    head->prompt = NULL;

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

        // Zeroing all values
        next->next = NULL;
        next->answer = NULL;
        next->prompt = NULL;

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

// Frees question set using Iteration
void free_qset(question *qset)
{
    question *temp = NULL;

    // Loop
    while (qset != NULL)
    {
        // Zeroing and freeing all values
        if (qset->answer != NULL)
        {
            free(qset->answer);
            qset->answer = NULL;
        }

        if (qset->prompt != NULL)
        {
            free(qset->prompt);
            qset->prompt = NULL;
        }

        // Freeing actual node
        temp = qset;
        qset = temp->next;

        free(temp);
        temp = NULL;
    }

    return;
}

// Fills out answer and prompt part of question, returns true or false based on execution
static bool set_question(question *question)
{
    // Choosing whether the question has a single topic or multiple
    bool single_topic = (bool) msws_uint(0, 1);

    switch (single_topic)
    {
        // Question generation for single topic questions
        case true:
            break;
        
        // Question generation for multi-topic questions
        case false:
            break;
    }

    return true;
}