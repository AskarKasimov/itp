#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

long long is_valid_number(const char *token, long long *out_value)
{
    if (token == NULL || *token == '\0')
        return 0;
    char *endptr;
    errno = 0;
    // конвертация
    long long val = strtoll(token, &endptr, 10);

    // проверка на фаззинг строками вмместо чисел
    if (*endptr != '\0' && *endptr != '\n')
    {
        return 0;
    }

    // проверка на переполнение(??)
    if (val < LLONG_MIN || val > LLONG_MAX)
    {
        return 0;
    }
    // ошибка strtol
    if (errno == ERANGE)
    {
        return 0;
    }

    if (out_value)
    {
        // записываю по указателю
        *out_value = (long long)val;
    }
    // true, если всё ок, т.е. значение лежит по указанию
    return 1;
}

int compareInts(const void *a, const void *b)
{
    // функция для безопасного сравнения целых чисел (для qsort по возрастанию)
    long long x = *(long long *)a;
    long long y = *(long long *)b;
    if (x < y)
        return -1;
    else if (x > y)
        return 1;
    else
        return 0;
}

int main()
{
    FILE *file = fopen("input.txt", "r");  // читка input
    FILE *fout = fopen("output.txt", "w"); // открыть output

    long long n = 0;
    fscanf(file, "%lld\n", &n); // первое число N - кол-во элементов
    if (n < 1 || n > 1000)
    {
        fprintf(fout, "Invalid input!");
        return 0;
    }

    char line[8192];
    if (fgets(line, sizeof(line), file)) // целиком вторая строка из файла
    {
        long long numbers[n]; // массив чисел
        long long iterator = 0;
        long long summa = 0; // сумма его элементов

        long long firstCoord = 0;
        long long i = 0;
        while (1)
        {
            if (line[i] == ',' || line[i] == '\0' || line[i] == '\n')
            {
                // разделитель или конец строки
                long long len = i - firstCoord;
                if (len <= 0)
                {
                    fprintf(fout, "Invalid input!");
                    return 0;
                }

                char token[64];
                if (len >= sizeof(token))
                {
                    fprintf(fout, "Invalid input!");
                    return 0;
                }
                memcpy(token, &line[firstCoord], len);
                token[len] = '\0';

                long long value;
                if (is_valid_number(token, &value))
                {
                    numbers[iterator++] = value;
                    summa += value;
                }
                else
                {
                    fprintf(fout, "Invalid input!");
                    return 0;
                }

                if (line[i] == '\0' || line[i] == '\n')
                {
                    break; // строка закончилась
                }

                // после запятой обязательно должен быть пробел
                if (line[i + 1] != ' ')
                {
                    fprintf(fout, "Invalid input!");
                    return 0;
                }

                firstCoord = i + 2; // начало следующего числа
                i += 2;
                continue;
            }

            if (!isdigit((unsigned char)line[i]) && line[i] != '-' && line[i] != '+')
            {
                fprintf(fout, "Invalid input!");
                return 0;
            }

            i++;
        }

        if (iterator != n)
        {
            // проверка количества элементов
            fprintf(fout, "Invalid input!");
            return 0;
        }

        // проверка, что дальше в файле нет мусора
        long long ch;
        while ((ch = fgetc(file)) != EOF)
        {
            if (!isspace(ch))
            { // любой символ - бан
                fprintf(fout, "Invalid input!");
                return 0;
            }
        }

        double mean = (double)summa / n; // вычисление ср.знач

        qsort(numbers, n, sizeof(long long), compareInts); // сортировка

        double median = 0;
        if (n % 2 == 1)
        { // проверка длины для медианы
            median = numbers[n / 2];
        }
        else
        {
            median = (numbers[n / 2] + numbers[n / 2 - 1]) / 2.0; // ср.знач двух серединных элементов
        }

        long long mode = numbers[0];
        long long maxSublistLen = 1;
        long long currentSublistLen = 1;
        for (long long i = 1; i < n; i++)
        {
            if (numbers[i] == numbers[i - 1])
            {
                currentSublistLen++;
            }
            else
            {
                currentSublistLen = 1;
            }

            if (currentSublistLen > maxSublistLen)
            {
                maxSublistLen = currentSublistLen;
                mode = numbers[i];
            }
            else if (currentSublistLen == maxSublistLen && numbers[i] < mode)
            {
                // если та же частота, но число меньше значит обнвить моду
                mode = numbers[i];
            }
        }

        fprintf(fout, "Mean: %.2f\n", mean);
        fprintf(fout, "Median: %.2f\n", median);
        fprintf(fout, "Mode: %lld\n", mode);
    }
    fclose(file);
    fclose(fout);

    return 0;
}
