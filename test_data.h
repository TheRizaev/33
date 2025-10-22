#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "algorithms.h"

typedef struct {
    char* name;
    Image* input;
    int target_rows;
    int target_cols;
    Image* expected_output;
} TestCase;

// Изменили первый параметр на const char*
TestCase* create_test_case(const char* name, int** data, int rows, int cols,
    int target_rows, int target_cols, int** expected);
void free_test_case(TestCase* test);

TestCase** get_all_tests(int* count);
void free_all_tests(TestCase** tests, int count);

#endif