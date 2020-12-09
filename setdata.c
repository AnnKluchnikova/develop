#include <stdio.h>
#include <stdio_ext.h> // для __fpurge()
#include <stdlib.h> // для atoi(), free()
#include <string.h> // для strchr(), strlen()

#include "types.h"

#define MAX_AGE 150  // Самому старому человеку в мире было 145 лет

//________________________________________________________________________ЧТЕНИЕ И ОБРАБОТКА ДАННЫХ
/*Функция для считывания данных*/
int read_data(char *buf, int data_len)
{
    char *trigger = NULL;

    fgets(buf, data_len, stdin);
    __fpurge(stdin);

    trigger = strchr(buf, '\n');       // Избавление от символа конца строки необходимо
                                      // во избежании проблем со сравнением данных,
    if (trigger != NULL)             // т.к. сравниваемые данные в основном не имеют такого символа
        *trigger = '\0';            // и могут отличаться только "\n" в конце строки
    else
    {
        printf("\nERROR! "
               "The string is longer than the expected size.\n");  // Но если в строке не найдется символа конца строки,
        return ERROR;                                             // значит строка была введена длиннее, чем ожидалось
    }

    return VALID;
}

/*Функция для проверки непустоты строки*/
int is_not_null_string(const char *buf)
{
    int simbols = 0;

    if(buf == NULL)        // Проверка на наличие пустой строки в непустом файле
        goto false_data;  // Без этого условия программа падает при обработке такой строки

    for(int index = 0; index < strlen(buf); index++) // Проверяется, что строка содержит значимые символы,
        if(buf[index] != ' ')                       // а не заполнена пробелами
            simbols++;                             // В базе не может быть пустых значений или только пробелы

    if(simbols == 0)
        goto false_data;

    return VALID;

false_data:
    printf("\nERROR! The string is empty.\n");
    return ERROR;
}

/*Функция проверки строковых данных на корректность*/
int is_correct_string(const char *buf)
{
    if(is_not_null_string(buf) != VALID)
        return ERROR;

    /*if(strchr(buf, ',') != NULL)    // Запятая является разделителем между разными данными в файле
        return ERROR;                // Если она окажется в принимаемой строке, то при сохранении в файл и
                                    // считывании из этого файла данные будут восприниматься не корректно
*/ // ATTANTION Если используется файл формата json, то это действие не нужно

    if(strlen(buf) >= STRING_LEN)
    {
        printf("\nERROR! The string is longer than 63 characters.\n");
        return ERROR;
    }

    return VALID;
}

/*Функция для преобразования возраста из строки в число*/
int get_age_as_number(const char *buf)
{
    int get_number = -1;

    if(is_not_null_string(buf) != VALID)
        return ERROR;

    get_number = atoi(buf);

    if((get_number > 0)&&(get_number < MAX_AGE))
        return get_number;
    else if(get_number == 0)        // Возраст "0" соответсвует ребенку,
        if(strcmp(buf, "0") == 0)  // не достигшему возраста 1 год
            return get_number;

    printf("\nERROR! Age doesn't match the real one.\n");
    return ERROR;
}

/*Функция для преобразования номера дома из строки в число*/
int get_home_number_as_number(const char *buf)
{
    int get_number = -1;

    if(is_not_null_string(buf) != VALID)
        return ERROR;

    if(strlen(buf) >= NUMBER_LEN)
    {
        printf("\nERROR! House number doesn't match the real one.\n");
        return ERROR;
    }

    get_number = atoi(buf);

    if(get_number > 0)
        return get_number;

    return VALID;
}