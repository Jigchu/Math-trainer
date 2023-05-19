#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "questions.h"

// Number of threads used by create_qset
#define THREAD_NUM 4

volatile static question *last_question = NULL;
pthread_mutex_t qset_mutex;

// Fills out answer and prompt part of question, returns true or false based on execution
static int set_question(question *question, int *topics)
{
    return 0;
}

// Generates question into qset in question.h. Can be multi-threaded.
static int qns_factory(create_arg args)
{
    int q_num = args.q_num;
    int q_count = 0;

    // Generates questions
    for (;q_count < q_num; q_count++)
    {
        // Generates question
        question *current_question = malloc(sizeof(question));
        current_question->next = NULL;
        current_question->answer = NULL;
        current_question->prompt = NULL;

        if (current_question == NULL)
        {
            return -1;
        }

        // Generate prompt and answers
        if (set_question(current_question, args.topic))
        {
            return -2;
        }

        // Add to question set
        pthread_mutex_lock(&qset_mutex);

        question *curr_last_question = (question *)last_question;
        if (curr_last_question == NULL)
        {
            curr_last_question = current_question;
            qset = current_question;
        }
        else
        {
            curr_last_question->next = current_question;
        }
        last_question = current_question;

        pthread_mutex_unlock(&qset_mutex);
    }

    return q_count;
}

// A function that manages qns_factory
int create_qset(create_arg args)
{
    int q_num = args.q_num, total_q = 0, retval = 0, q_per_thread[THREAD_NUM];;
    pthread_t thread_ids[THREAD_NUM];
    create_arg thread_args;

    // Initialise mutex
    pthread_mutex_init(&qset_mutex, NULL);

    do
    {
        // Calculate the number of questions to be generated per thread
        int qns_to_create = q_num - total_q;

        // Checking for remainder
        int remainder = qns_to_create % THREAD_NUM;

        for (int i = 0; i < THREAD_NUM; i++)
        {
            q_per_thread[i] = qns_to_create / THREAD_NUM;
        }
        
        for (int i = 0; i < remainder; i++)
        {
            q_per_thread[i]++;
        }
        
        // Create threads
        for (int i = 0; i < THREAD_NUM; i++)
        {
            create_arg thread_args = {args.topic, q_per_thread[i]};

            // Error detection
            if (pthread_create(&thread_ids[i], NULL, &qns_factory, &thread_args))
            {
                return -1;
            }
        }

        // Terminating threads
        for (int i = 0; i < THREAD_NUM; i++)
        {
            // Error detection
            if (pthread_join(thread_ids[i], &retval))
            {
                return -2;
            }

            if (retval == -1)
            {
                return -3;
            }
            else if (retval == -2)
            {
                return -4;
            }
            
            // Update the total amount of questions generated
            total_q += retval;
        }
    } while (total_q != q_num);
    

    // Destroy mutex
    pthread_mutex_destroy(&qset_mutex);

    return total_q;
}

// Frees question set using Iteration
void free_qset()
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