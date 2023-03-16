#include <stdio.h>

#include "../questions.h"
#include "../questions.c"
#include "../colours.h"

void create_t(int q_num);

int main(void)
{
    create_t(10);
    create_t(100);

    return 0;
}


// Tests if create_qset is able to able to allocate memory for question nodes (Does not check contents)
void create_t(int q_num)
{
    question *qset = create_qset("q", q_num);

    int qctr = 0;

    question *qptr = qset;

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

    return;
}