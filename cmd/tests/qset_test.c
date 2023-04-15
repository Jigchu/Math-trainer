#include <stdio.h>
#include <stdlib.h>

#include "../src/questions.c"

#include "tests/qset_test.h"

#include "colours.h"
#include "questions.h"


// Tests if create_qset is able to able to allocate memory for question nodes (Does not check contents)
void create_t(int q_num)
{
    // Question generation
    int topics[] = {'q'};

    question *qset = create_qset(topics, q_num);

    if (qset == NULL)
    {
        red();
        fprintf(stdout, ":( Memory could not be allocated succesfully\n");
        reset();
    }

    int qctr = 0;

    question *qptr = qset;

    // Making sure all questions were generated
    while(qptr)
    {
        qptr = qptr->next;
        qctr++;
    }

    if (qctr == q_num)
    {
        green();
        fprintf(stdout, ":) create_qset able to generate %d questions successfully\n", q_num);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( create_qset unable to generate %d questions successfully. Only generated %d questions\n", q_num, qctr);
        reset();
    }

    free_qset(qset);

    return;
}

// A stress test for free_qset
void free_t(int q_num)
{
    // Generating questions
    int topics[] = {110};
    question *qset = create_qset(topics , q_num);

    // Freeing questions
    free_qset(qset);

    green();
    fprintf(stdout, ":) free_qset() successfully executed with %d questions\n", q_num);
    reset();

    return;
}