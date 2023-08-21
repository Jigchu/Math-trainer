#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../tests/include/qset_test.h"
#include "../tests/include/rng_test.h"

#include "random.h"

int main(int argc, char * const argv[])
{
    // Enable all feature
    bool all = (argc == 2) && (!strcmp(argv[argc - 1], "--all")) ? true : false;

    srand(time(NULL));
    seed = msws_init(true);

    // Current tests (These tests are for functions that have not been fully tested yet)
    // Tests for question set generation
    create_t(100000);

    // Stress tests for memory deallocation
    free_t(100000);

    // Rest of the tests (Fully tested functions)
    if (all)
    {
        // Tests to make sure seed.h is correctly generated
        seed_t();

        // Tests to ensure the rng actually works
        rng32inv_t();
        rng64inv_t();

        rng32_t(4121, 324123);
        rng32_t(0, RAND32MAX - 1);

        rng64_t(2421, 4234232);
        rng64_t(0, RAND64MAX - 1);
        
        // Test for float rng
        rngflt_t();
    }

    free_rng(seed);

    return 0;
}