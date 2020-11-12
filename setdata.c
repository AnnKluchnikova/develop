#include <stdio.h>
#include <stdio_ext.h> // для __fpurge()
#include <stdlib.h> // для atoi(), free()
#include <string.h> // для strchr(), strlen()

#include "types.h"
#include "lstedit.h"
//________________________________________________________________________ЧТЕНИЕ И ОБРАБОТКА ДАННЫХ
/*Функция для считывания данных*/
int read_data(char *buf, int data_len)
{
    fgets(buf, data_len, stdin);
    __fpurge(stdin);

    char *trigger = strchr(buf, '\n'); // Избавление от символа конца строки необходимо
                                      // во избежания проблем со сравнением данных,
    if (trigger != NULL)             // т.к. сравниваемые данные в основном не имеют такого символа
        *trigger = '\0';            // и могут отличаться только "\n" в конце строки
    else
        goto false_data;          // Но если в строке не найдется символа конца строки,
                                 // значит строка была введена длиннее, чем ожидалось

    return VALID;

false_data:
    printf("\nERROR! The string is longer than the expected size.\n");
    return ERROR;
}

/*Функция для проверки непустоты строки*/
int is_not_null_string(char *buf)
{
    if(buf == NULL)        // Проверка на наличие пустой строки в непустом файле
        goto false_data;  // Без этого условия программа падает при обработке такой строки

    int simbols = 0;
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
int is_correct_string(char *buf)
{
    if(is_not_null_string(buf) != VALID)
        return ERROR;

    if(strchr(buf, ',') != NULL)    // Запятая является разделителем между разными данными в файле
        goto false_data;           // Если она окажется в принимаемой строке, то при сохранении в файл и
                                  // считывании из этого файла данные будут восприниматься не корректно

    if(strlen(buf) >= STRING_LEN)
        goto false_data;

    return VALID;

false_data:
    printf("\nERROR! The string is longer than 63 characters or contains ','.\n");
    return ERROR;
}

/*Функция для распознования возраста "0 лет"*/
int zero_is_age(char *buf)
{
    int index = 0;

    while(index < strlen(buf))
    {
        if(buf[index] == '0')
            return VALID;
        index++;
    }

    return ERROR;
}

/*Функция для преобразования возраста из строки в число*/
int get_age_as_number(char *buf)
{
    if(is_not_null_string(buf) != VALID)
        return ERROR;

    int get_number = atoi(buf);

    if((get_number > 0)&&(get_number < 150)) // Самому старому человеку в мире было 145 лет
        return get_number;
    else if(get_number == 0)        // Возраст "0" соответсвует младенцу,
        if(zero_is_age(buf) == VALID)  // не достигшему возраста 1 год
            return get_number;

    printf("\nERROR! Age doesn't match the real one.\n");
    return ERROR;
}

/*Функция для преобразования номера дома из строки в число*/
int get_home_num_as_number(char *buf)
{
    if(is_not_null_string(buf) != VALID)
        goto false_data;

    int get_number = atoi(buf);

    if(get_number > 0)
        return get_number;

false_data:
    printf("\nERROR! House number doesn't match the real one.\n");
    return ERROR;
}

/*Функция проверки полученных данных для последующего добавления*/
int add_correct_data_to_database(char *name, char *age, char *street_name, char *home_num, int call)
{
    if(is_correct_string(name) == ERROR)
        goto false_data;

    int ret_age = get_age_as_number(age);
    if (ret_age == ERROR)
        goto false_data;

    if(is_correct_string(street_name) == ERROR)
        goto false_data;

    int ret_home_num = get_home_num_as_number(home_num);
    if(ret_home_num == ERROR)
        goto false_data;

    add_new_person(name, ret_age, street_name, ret_home_num, call);

    return VALID;

false_data:
    return ERROR;
}
