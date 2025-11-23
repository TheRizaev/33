#include <stdio.h>
#include "tests.h"
#include "algorithms.h"
#include <string.h>
#include <stdlib.h>
#include <clocale>

void interactive_mode() {
    printf("Интерактивный режим\n");
    printf("Введите матрицу изображения (завершите ввод строкой с двумя числами k1 m1):\n");

    int k0 = 0, m0 = 0, k1, m1;
    char line[10000];

    char** lines = (char**)malloc(1000 * sizeof(char*));
    int max_lines = 1000;

    while (fgets(line, sizeof(line), stdin)) {
        int temp1, temp2;
        char extra;
        if (sscanf(line, "%d %d %c", &temp1, &temp2, &extra) == 2) {
            k1 = temp1;
            m1 = temp2;
            break;
        }

        if (k0 >= max_lines) {
            max_lines *= 2;
            lines = (char**)realloc(lines, max_lines * sizeof(char*));
        }
        lines[k0] = (char*)malloc(strlen(line) + 1);
        strcpy(lines[k0], line);
        k0++;
    }

    if (k0 > 0) {
        char* temp = (char*)malloc(strlen(lines[0]) + 1);
        strcpy(temp, lines[0]);
        char* token = strtok(temp, " \t\n");
        while (token != NULL) {
            m0++;
            token = strtok(NULL, " \t\n");
        }
        free(temp);
    }

    Image* image = create_image(k0, m0);

    for (int i = 0; i < k0; i++) {
        char* token = strtok(lines[i], " \t\n");
        int j = 0;
        while (token != NULL && j < m0) {
            image->data[i][j] = atoi(token);
            j++;
            token = strtok(NULL, " \t\n");
        }
        free(lines[i]);
    }
    free(lines);

    printf("\nВыберите алгоритм:\n");
    printf("1 - Взвешенное усреднение (более точный)\n");
    printf("2 - Простое усреднение (более быстрый)\n");
    printf("Ваш выбор: ");

    int choice;
    scanf("%d", &choice);

    Image* result;
    if (choice == 1) {
        result = algorithm1_weighted_averaging(image, k1, m1);
    }
    else {
        result = algorithm2_simple_averaging(image, k1, m1);
    }

    printf("\nРезультат:\n");
    print_image(result);

    free_image(image);
    free_image(result);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
;    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        run_all_tests();
    }
    else if (argc > 1 && strcmp(argv[1], "interactive") == 0) {
        interactive_mode();
    }
    else {
        printf("Использование:\n");
        printf("  %s test         - запустить все тесты\n", argv[0]);
        printf("  %s interactive  - интерактивный режим\n", argv[0]);
        printf("\nЗапуск тестов по умолчанию...\n\n");
        run_all_tests();
    }

    return 0;
}