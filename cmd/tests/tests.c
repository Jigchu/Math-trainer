#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../mswsrngv5/msws32.h"
#include "../mswsrngv5/msws64.h"

#include "../colours.h"
#include "../questions.h"
#include "../questions.c"
#include "../random.h" 
#include "../random.c"

void create_t(int q_num);
void free_t(int q_num);
void seed_t(void);
void rng32_t(uint32_t max);
void rng64_t(uint64_t max);

int main(void)
{
    srand(time(NULL));

    // Tests for question set generation
    create_t(1);
    create_t(10);
    create_t(100);
    create_t(1000);

    // Stress tests for memory deallocation
    free_t(1);
    free_t(10);
    free_t(100);
    free_t(1000);

    // Tests to make sure seed.h is correctly generated
    seed_t();

    // Tests to ensure the rng actually works
    rng32_t(1);
    rng32_t(RAND32MAX - 1);
    rng32_t(24908);

    rng64_t(1);
    rng64_t(RAND64MAX - 1);
    rng64_t(2491024);

    return 0;
}


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

// A test to check whether the functions generating and checking seed.h works
void seed_t(void)
{
    bool corrupted = seed_verifier();

    if (corrupted)
    {
        red();
        fprintf(stdout, ":( Seed verifier does not work\n");
        fprintf(stdout, "Cannot proceed with seed_t's subsequent tests\n");
        reset();

        return;
    }
    else
    {
        green();
        fprintf(stdout, ":) Seed verifier works\n");
        reset();
    }

    gen_seed();

    corrupted = seed_verifier();

    if (corrupted)
    {
        red();
        fprintf(stdout, ":( Seed generator does not work\n");
        reset();
    }
    else
    {
        green();
        fprintf(stdout, ":) Seed generator works\n");
        reset();
    }

    return;
}

// Makes sure the eng works
void rng32_t(uint32_t max)
{
    msws32_s();

    uint32_t result = msws_uint(RAND32MAX);

    if (result == RAND32MAX)
    {
        green();
        fprintf(stdout, ":) msws32 RNG able to handle an invalid input\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws32 RNG unable to handle an invalid input. Output: %u\n", result);
        reset();
    }

    result = msws_uint(max);

    if (result <= max)
    {
        green();
        fprintf(stdout, ":) msws32 RNG works. Result: %u\n", result);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws32 RNG does not work. max: %u, result: %u\n", max, result);
        reset();
    }

    return;
}

void rng64_t(uint64_t max)
{
    msws64_s();

    uint64_t result = msws_ull(RAND64MAX);

    if (result == RAND64MAX)
    {
        green();
        fprintf(stdout, ":) msws64 RNG able to handle an invalid input\n");
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws64 RNG unable to handle an invalid input. Output: %llu\n", result);
        reset();
    }

    result = msws_ull(max);

    if (result <= max)
    {
        green();
        fprintf(stdout, ":) msws64 RNG works. Result: %llu\n", result);
        reset();
    }
    else
    {
        red();
        fprintf(stdout, ":( msws64 RNG does not work. max: %llu, result: %llu\n", max, result);
        reset();
    }

    return;
}