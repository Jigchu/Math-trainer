#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "questions.h"

question *create_qsets(int *topics, int max, int min, int max_q);
void free_qset(question *qset, int max_q);
