// Header file for question manipulation and generation
#pragma once

// Struct for question
typedef struct question question;

// Struct when using create_q with multi-threading
typedef struct create_arg
{
    int *topic;
    int q_num;
} create_arg;

// Question set
extern question *qset;

// Function prototypes
int create_qset(create_arg args);
void free_qset(void);