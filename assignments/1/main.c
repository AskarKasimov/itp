#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

int is_valid_number(const char *token, int *out_value) {
    if (token == NULL || *token == '\0')
        return 0;
    char *endptr;
    errno = 0;
    // конвертация
    int val = strtol(token, &endptr, 10);
    // вся строка должна быть числом
    if (*endptr != '\0') {
        return 0; // есть лишние символы
    }

    // проверка на переполнение(??)
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    // ошибка strtol
    if (errno == ERANGE) {
        return 0;
    }

    if (out_value) {
        // записываю по указателю
        *out_value = (int)val;
    }
    // true, если всё ок, т.е. значение лежит по указанию
    return 1;
}

int compareInts(const void* a, const void* b) {
    // функция для сравнения целых чисел (для qsort по убыванию)
   return (*(int*)b - *(int*)a);
}

int main() {
    FILE *file = fopen("input.txt", "r"); // читка input
    FILE *fout = fopen("output.txt", "w"); // открыть output

    int n;
    fscanf(file, "%d\n", &n); // первое число N - кол-во элементов
    if (n < 1) {
        fprintf(fout, "Invalid input!");
        return 0;
    }

    char line[8192];
    if (fgets(line, sizeof(line), file)) { // целиком вторая строка
        int numbers[n]; // основной массив
        int summa = 0; // сумма его элементов

        char *token = strtok(line, ","); // сплит по запятой и пробелу
        if (token == NULL) {
            fprintf(fout, "Invalid input!");
            return 0;
        }

        int index = 0;
        while (token != NULL && index < n) {
            int value;
            if (is_valid_number(token, &value)) {
                numbers[index++] = value;
                summa += value;
            } else {
                fprintf(fout, "Invalid input!");
                return 0;
            }
            token = strtok(NULL, ",");
        }

        if (token != NULL && index >= n) {
            // проверка если остались лишние элементы
            fprintf(fout, "Invalid input!");
            return 0;
        }
        if (token == NULL && index < n) {
            // проверка если элементов не хватает
            fprintf(fout, "Invalid input!");
            return 0;
        }
        float mean = (float) summa / n; // вычисление ср.знач

        qsort (numbers, n, sizeof(int), compareInts); // сортировка

        float median = 0;
        if (n % 2 == 1) { // проверка длины для медианы
            median = numbers[n / 2];
        } else {
            median = (numbers[n/2] + numbers[n/2-1]) / 2.0; // ср.знач двух серединных элементов
        }

        int mode = numbers[n - 1];
        // "хитрый" подсчет однородных подотрезков за O(n) по времени и O(1) по памяти. раз уж массив всё равно отсортирован для нахождения медианы, подход с мапой будет дороже
        int isStreak = 0; // bool, указывающая на однородность подотрезка
        int maxSublistLen = 0; // максимальная длина подотрезка
        int currentSublistLen = 0; // текущая
        for (int i = 1; i < n; i ++) {
            if (isStreak == 0) currentSublistLen = 0;
            if (numbers[i] == numbers[i-1]) {
                isStreak = 1;
                currentSublistLen ++;
                if (currentSublistLen >= maxSublistLen) { // текущий отрезок >= максимальному значит обновляем. т.к. массив специально отсортирован ПО УБЫВАНИЮ, последнее значение моды ряда будет всегда наименьшим из максимальных по кол-ву
                    maxSublistLen = currentSublistLen;
                    mode = numbers[i];
            }
            } else isStreak = 0;
        }
        
        fprintf(fout, "Mean: %.2f\n", mean);
        fprintf(fout, "Median: %.2f\n", median);
        fprintf(fout, "Mode: %d\n", mode);
    }
    fclose(file);
    fclose(fout);

    return 0;
}
