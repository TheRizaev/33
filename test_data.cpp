#include "test_data.h"
#include <stdlib.h>
#include <string.h>

TestCase* create_test_case(const char* name, int** data, int rows, int cols,
    int target_rows, int target_cols, int** expected) {
    TestCase* test = (TestCase*)malloc(sizeof(TestCase));
    test->name = (char*)malloc(strlen(name) + 1);
    strcpy(test->name, name);

    test->input = create_image(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            test->input->data[i][j] = data[i][j];
        }
    }

    test->target_rows = target_rows;
    test->target_cols = target_cols;

    if (expected != NULL) {
        test->expected_output = create_image(target_rows, target_cols);
        for (int i = 0; i < target_rows; i++) {
            for (int j = 0; j < target_cols; j++) {
                test->expected_output->data[i][j] = expected[i][j];
            }
        }
    }
    else {
        test->expected_output = NULL;
    }

    return test;
}

void free_test_case(TestCase* test) {
    if (test == NULL) return;
    free(test->name);
    free_image(test->input);
    if (test->expected_output != NULL) {
        free_image(test->expected_output);
    }
    free(test);
}

TestCase** get_all_tests(int* count) {
    // Тест 1: Сжатие 4x4 -> 2x3
    int** test1_input = (int**)malloc(4 * sizeof(int*));
    int test1_data[4][4] = {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 1, 1},
        {1, 0, 1, 0}
    };
    for (int i = 0; i < 4; i++) {
        test1_input[i] = (int*)malloc(4 * sizeof(int));
        for (int j = 0; j < 4; j++) {
            test1_input[i][j] = test1_data[i][j];
        }
    }

    // Тест 2: Растяжение 2x2 -> 4x4
    int** test2_input = (int**)malloc(2 * sizeof(int*));
    int test2_data[2][2] = {
        {1, 0},
        {0, 1}
    };
    for (int i = 0; i < 2; i++) {
        test2_input[i] = (int*)malloc(2 * sizeof(int));
        for (int j = 0; j < 2; j++) {
            test2_input[i][j] = test2_data[i][j];
        }
    }

    // Тест 3: СРЕДНИЙ - Сжатие 1000x1000 -> 100x100
    int** test3_input = (int**)malloc(1000 * sizeof(int*));
    for (int i = 0; i < 1000; i++) {
        test3_input[i] = (int*)malloc(1000 * sizeof(int));
        for (int j = 0; j < 1000; j++) {
            // Шахматный узор с большими клетками
            test3_input[i][j] = ((i / 50) + (j / 50)) % 2;
        }
    }

    // Тест 4: СРЕДНИЙ - Растяжение 100x100 -> 1000x1000
    int** test4_input = (int**)malloc(100 * sizeof(int*));
    for (int i = 0; i < 100; i++) {
        test4_input[i] = (int*)malloc(100 * sizeof(int));
        for (int j = 0; j < 100; j++) {
            // Круги
            int dx = i - 50;
            int dy = j - 50;
            test4_input[i][j] = (dx * dx + dy * dy < 1500) ? 1 : 0;
        }
    }

    // Тест 5: БОЛЬШОЙ - Сжатие 2000x2000 -> 200x200
    int** test5_input = (int**)malloc(2000 * sizeof(int*));
    for (int i = 0; i < 2000; i++) {
        test5_input[i] = (int*)malloc(2000 * sizeof(int));
        for (int j = 0; j < 2000; j++) {
            // Диагональные полосы
            test5_input[i][j] = ((i + j) / 100) % 2;
        }
    }

    // Тест 6: БОЛЬШОЙ - Растяжение 200x200 -> 2000x2000
    int** test6_input = (int**)malloc(200 * sizeof(int*));
    for (int i = 0; i < 200; i++) {
        test6_input[i] = (int*)malloc(200 * sizeof(int));
        for (int j = 0; j < 200; j++) {
            // Концентрические квадраты
            int dist = (i < 100 ? i : 199 - i);
            int dist2 = (j < 100 ? j : 199 - j);
            int min_dist = (dist < dist2 ? dist : dist2);
            test6_input[i][j] = (min_dist / 20) % 2;
        }
    }

    // Тест 7: ОЧЕНЬ БОЛЬШОЙ - Сжатие 3000x3000 -> 300x300
    int** test7_input = (int**)malloc(3000 * sizeof(int*));
    for (int i = 0; i < 3000; i++) {
        test7_input[i] = (int*)malloc(3000 * sizeof(int));
        for (int j = 0; j < 3000; j++) {
            // Сложный узор
            test7_input[i][j] = ((i * j) / 10000) % 2;
        }
    }

    // Тест 8: ОЧЕНЬ БОЛЬШОЙ - Растяжение 300x300 -> 3000x3000
    int** test8_input = (int**)malloc(300 * sizeof(int*));
    for (int i = 0; i < 300; i++) {
        test8_input[i] = (int*)malloc(300 * sizeof(int));
        for (int j = 0; j < 300; j++) {
            // Волны
            test8_input[i][j] = ((i / 30) + (j / 30)) % 2;
        }
    }

    // Тест 9: ЭКСТРЕМАЛЬНЫЙ - Сжатие 4000x4000 -> 400x400
    int** test9_input = (int**)malloc(4000 * sizeof(int*));
    for (int i = 0; i < 4000; i++) {
        test9_input[i] = (int*)malloc(4000 * sizeof(int));
        for (int j = 0; j < 4000; j++) {
            // Градиент
            test9_input[i][j] = (i + j) % 2;
        }
    }

    // Тест 10: ЭКСТРЕМАЛЬНЫЙ - Растяжение 400x400 -> 4000x4000
    int** test10_input = (int**)malloc(400 * sizeof(int*));
    for (int i = 0; i < 400; i++) {
        test10_input[i] = (int*)malloc(400 * sizeof(int));
        for (int j = 0; j < 400; j++) {
            // Спираль-подобный узор
            test10_input[i][j] = ((i / 40) ^ (j / 40)) % 2;
        }
    }

    // Тест 11: МЕГА - Сжатие 5000x5000 -> 500x500
    int** test11_input = (int**)malloc(5000 * sizeof(int*));
    for (int i = 0; i < 5000; i++) {
        test11_input[i] = (int*)malloc(5000 * sizeof(int));
        for (int j = 0; j < 5000; j++) {
            // Сложные полосы
            test11_input[i][j] = ((i / 200) + (j / 300)) % 2;
        }
    }

    // Тест 12: МЕГА - Растяжение 500x500 -> 5000x5000
    int** test12_input = (int**)malloc(500 * sizeof(int*));
    for (int i = 0; i < 500; i++) {
        test12_input[i] = (int*)malloc(500 * sizeof(int));
        for (int j = 0; j < 500; j++) {
            // Клеточный узор
            test12_input[i][j] = ((i / 50) + (j / 50)) % 2;
        }
    }

    // Тест 13: ГИГА - Сжатие 6000x6000 -> 600x600
    int** test13_input = (int**)malloc(6000 * sizeof(int*));
    for (int i = 0; i < 6000; i++) {
        test13_input[i] = (int*)malloc(6000 * sizeof(int));
        for (int j = 0; j < 6000; j++) {
            test13_input[i][j] = (i ^ j) % 2;
        }
    }

    // Тест 14: Сильное сжатие 5000x5000 -> 50x50
    int** test14_input = (int**)malloc(5000 * sizeof(int*));
    for (int i = 0; i < 5000; i++) {
        test14_input[i] = (int*)malloc(5000 * sizeof(int));
        for (int j = 0; j < 5000; j++) {
            test14_input[i][j] = ((i / 500) + (j / 500)) % 2;
        }
    }

    // Тест 15: Сильное растяжение 50x50 -> 5000x5000
    int** test15_input = (int**)malloc(50 * sizeof(int*));
    for (int i = 0; i < 50; i++) {
        test15_input[i] = (int*)malloc(50 * sizeof(int));
        for (int j = 0; j < 50; j++) {
            int dx = i - 25;
            int dy = j - 25;
            test15_input[i][j] = (dx * dx + dy * dy < 400) ? 1 : 0;
        }
    }

    *count = 15;
    TestCase** tests = (TestCase**)malloc((*count) * sizeof(TestCase*));

    tests[0] = create_test_case("Bazovyy: Szhatie 4x4 -> 2x3", test1_input, 4, 4, 2, 3, NULL);
    tests[1] = create_test_case("Bazovyy: Rastyazhenie 2x2 -> 4x4", test2_input, 2, 2, 4, 4, NULL);
    tests[2] = create_test_case("SREDNIY: Szhatie 1000x1000 -> 100x100", test3_input, 1000, 1000, 100, 100, NULL);
    tests[3] = create_test_case("SREDNIY: Rastyazhenie 100x100 -> 1000x1000", test4_input, 100, 100, 1000, 1000, NULL);
    tests[4] = create_test_case("BOLSHOY: Szhatie 2000x2000 -> 200x200", test5_input, 2000, 2000, 200, 200, NULL);
    tests[5] = create_test_case("BOLSHOY: Rastyazhenie 200x200 -> 2000x2000", test6_input, 200, 200, 2000, 2000, NULL);
    tests[6] = create_test_case("OCHEN BOLSHOY: Szhatie 3000x3000 -> 300x300", test7_input, 3000, 3000, 300, 300, NULL);
    tests[7] = create_test_case("OCHEN BOLSHOY: Rastyazhenie 300x300 -> 3000x3000", test8_input, 300, 300, 3000, 3000, NULL);
    tests[8] = create_test_case("EKSTREMALNYY: Szhatie 4000x4000 -> 400x400", test9_input, 4000, 4000, 400, 400, NULL);
    tests[9] = create_test_case("EKSTREMALNYY: Rastyazhenie 400x400 -> 4000x4000", test10_input, 400, 400, 4000, 4000, NULL);
    tests[10] = create_test_case("MEGA: Szhatie 5000x5000 -> 500x500", test11_input, 5000, 5000, 500, 500, NULL);
    tests[11] = create_test_case("MEGA: Rastyazhenie 500x500 -> 5000x5000", test12_input, 500, 500, 5000, 5000, NULL);
    tests[12] = create_test_case("GIGA: Szhatie 6000x6000 -> 600x600", test13_input, 6000, 6000, 600, 600, NULL);
    tests[13] = create_test_case("INTENSIVNYY: Silnoye szhatie 5000x5000 -> 50x50", test14_input, 5000, 5000, 50, 50, NULL);
    tests[14] = create_test_case("INTENSIVNYY: Silnoye rastyazhenie 50x50 -> 5000x5000", test15_input, 50, 50, 5000, 5000, NULL);

    // Освобождаем временные массивы
    for (int i = 0; i < 4; i++) free(test1_input[i]);
    free(test1_input);

    for (int i = 0; i < 2; i++) free(test2_input[i]);
    free(test2_input);

    for (int i = 0; i < 1000; i++) free(test3_input[i]);
    free(test3_input);

    for (int i = 0; i < 100; i++) free(test4_input[i]);
    free(test4_input);

    for (int i = 0; i < 2000; i++) free(test5_input[i]);
    free(test5_input);

    for (int i = 0; i < 200; i++) free(test6_input[i]);
    free(test6_input);

    for (int i = 0; i < 3000; i++) free(test7_input[i]);
    free(test7_input);

    for (int i = 0; i < 300; i++) free(test8_input[i]);
    free(test8_input);

    for (int i = 0; i < 4000; i++) free(test9_input[i]);
    free(test9_input);

    for (int i = 0; i < 400; i++) free(test10_input[i]);
    free(test10_input);

    for (int i = 0; i < 5000; i++) free(test11_input[i]);
    free(test11_input);

    for (int i = 0; i < 500; i++) free(test12_input[i]);
    free(test12_input);

    for (int i = 0; i < 6000; i++) free(test13_input[i]);
    free(test13_input);

    for (int i = 0; i < 5000; i++) free(test14_input[i]);
    free(test14_input);

    for (int i = 0; i < 50; i++) free(test15_input[i]);
    free(test15_input);

    return tests;
}

void free_all_tests(TestCase** tests, int count) {
    for (int i = 0; i < count; i++) {
        free_test_case(tests[i]);
    }
    free(tests);
}