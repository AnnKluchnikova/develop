#include <stdio.h>
#include "/usr/include/json-c/json.h"
#include <string.h> // для strcpy(), для strcmp()
#include <stdlib.h> // для calloc()

#include "types.h"
#include "listedit.h"

#define ADDRESS_FIELDS_NUMBER 2 // streetName, homeNumber
#define PERSON_FIELDS_NUMBER 3 // name, age, address

/*
Нужно использовать формат json для твоей базы данных.
*Т.е. тебе нужно использовать стороннюю библиотеку для парсинга 
*и сохранения файла. Возможно потребуется изменение структуры списка.
*/

/*Функция записывает значения объекта json в массив, при этом проверяя
соответсвие названия ключа. В конце выводит на экран связку "ключ:значение".
Так как функция будет вызываться в самом начале работы программы, вывод полученных
данных будет удобен для пользователя. Он сможет сразу проанализировать полученную
таблицу или увидеть на чтении какой строки произошла ошибка.*/
int write_values_to_array(struct json_object_iterator* itr,
                          char *array_to_write, char *json_key_name)
{
    if(strcmp(json_object_iter_peek_name(itr), json_key_name) != 0)
    {
        printf("The key is named \"%s\" but \"%s\" was expected.\n",
               json_object_iter_peek_name(itr), json_key_name);
        return ERROR;
    }

    struct json_object *value;
    value = json_object_iter_peek_value(itr);

    const char *json_string_value = json_object_get_string(value);

    strcpy(array_to_write, json_string_value);

    printf("%s:\t", json_object_iter_peek_name(itr));
    printf("%s\t", json_string_value);

    return VALID;
}

int get_name(struct json_object_iterator* itr, char *name)
{
    return write_values_to_array(itr, name, "Name");
}

int get_age(struct json_object_iterator* itr, char *age)
{
    return write_values_to_array(itr, age, "Age");
}

int get_street_name(struct json_object_iterator* itr, char *street_name)
{
    return write_values_to_array(itr, street_name, "Street Name");
}

int get_home_number(struct json_object_iterator* itr, char *home_num)
{
    return write_values_to_array(itr, home_num, "Home Number");
}

/*Функция для получения информации об адресе, в частности название улицы и 
номер дома, которые будут записаны в соответствующие массивы*/
int get_address_information(struct json_object *obj, char *street_name, char *home_num)
{
    if(json_object_object_length(obj) != ADDRESS_FIELDS_NUMBER)
    {
        printf("ERROR! The received values do not match the expected values.\n");
        return ERROR;
    }

    struct json_object_iterator* itr;
    struct json_object_iterator itr_struct;

    itr_struct = json_object_iter_begin(obj);
    itr = &itr_struct;

    if(get_street_name(itr, street_name) != VALID)
        goto false_data;

    json_object_iter_next(itr);

    if(get_home_number(itr, home_num) != VALID)
        goto false_data;

    return VALID;

false_data:
    return ERROR;
}

/*Функция записывает в память(массивы) отдельные значения из объекта json.
Предполагается, что дальше их будут передавать в функцию добавления элемента*/
int get_value_from_json_object(struct json_object *obj,
                               char *name, char *age,
                               char *street_name, char *home_num)
{
    struct json_object_iterator* itr;
    struct json_object_iterator itr_struct;

    if(json_object_object_length(obj) != PERSON_FIELDS_NUMBER)
    {
        printf("ERROR! The received values do not match the expected values.\n");
        return ERROR;
    }

    itr_struct = json_object_iter_begin(obj);
    itr = &itr_struct;

    if(get_name(itr, name) != VALID)
        goto false_data;

    json_object_iter_next(itr);

    if(get_age(itr, age) != VALID)
        goto false_data;

    json_object_iter_next(itr);

    if(get_address_information(json_object_iter_peek_value(itr),
                   street_name, home_num) != VALID)
        goto false_data;

    return VALID;

false_data:
    return ERROR;
}

/*Функция выделяет объект json из главного объекта
и передает его в функцию для получения хранимых значений, после чего
переводит значение итератора на следующий объект в составе главного*/
int get_data_from_json_object(struct json_object *obj, int obj_num, int obj_len,
                              char *name, char *age,
                              char *street_name, char *home_num)
{
    struct json_object *value;
    struct json_object_iterator *itr_struct;
    static struct json_object_iterator* itr; // При повторном вызове функции,
                                            // обработка должна начинаться со следующего элемента.
                                           // Поэтому нужно постоянно хранить адрес следующего.

    if(obj_num == 0)
    {
        itr_struct = calloc(1, sizeof(struct json_object_iterator));
        *itr_struct = json_object_iter_begin(obj);
        itr = itr_struct;
    }

    value = json_object_iter_peek_value(itr);
    int value_len = json_object_object_length(value);

    if(value_len == 0)     // TODO подумать над тем, что делать, если объект пуст
        goto false_data;

    printf("Object %s:\t", json_object_iter_peek_name(itr));

    if(get_value_from_json_object(value, name, age,
                                  street_name, home_num) != VALID)
        goto false_data;

    printf("\n");

    if(obj_num < obj_len - 1)          // Если обрабатывался не последний элемент в объекте,
        json_object_iter_next(itr);   // то перейти к следующему. Это необходимо, чтобы избежать перехода в NULL
    else                             // Если прошел процесс обработки последнего элемента,
        free(itr);                  // то нужно освободить выделенное calloc'ом место

    return VALID;

false_data:
    printf("ERROR! Empty object detected.\n");
    free(itr);
    return ERROR;
}

/*Функция очищает массивы от предыдущих записей*/
void delete_arrays_values(char *name, char *age, char *street_name, char *home_num)
{
    memset(name, 0, strlen(name));
    memset(age, 0, strlen(age));
    memset(street_name, 0, strlen(street_name));
    memset(home_num, 0, strlen(home_num));
}

/*Функция считывает данные файла json и записывает их во внутреннюю структуру*/
int add_json_object_to_database(char const *file_path)
{
    char name[STRING_LEN], age[NUMBER_LEN], street_name[STRING_LEN], home_num[NUMBER_LEN];

    struct json_object *obj;
    obj = json_object_from_file(file_path); // TODO Функция не дает ответа пуст ли файл,
                                           // или ошибка в синтаксисе. Единым решением будет
                                          // просто завершить программу с рекомендациями.

    if(obj == NULL)
    {
        printf("ERROR! Check the file \"%s\" and try again.\n"
               "Help: If the file is empty, "
               "just insert '{}' into the file yourself. "
               "If the file has data, check its spelling for json compliance.", file_path);
        return ERROR;
    }

    int obj_len = json_object_object_length(obj); // Определим количество объектов,
                                                 // т.е. кол-во людей в списке

    if(obj_len == 0)
    {
        printf("\nATTENTION! There is no data in the file.\n");
        goto valid_data;
    }

    for(int index = 0; index < obj_len; index++)
    {
        if(get_data_from_json_object(obj, index, obj_len,
                                  name, age, street_name, home_num) != VALID)
            goto false_data;

        if(add_correct_data_to_database(name, age, street_name, home_num,
                                        NO_MESSAGE) != VALID)
            goto false_data;

        delete_arrays_values(name, age, street_name, home_num);
    }

valid_data:
    //printf("\n Ready: %d entries were recorded from the file.\n", obj_len + 1);
    json_object_put(obj);
    return VALID;

false_data:
    clear_all_lists(NO_MESSAGE);
    json_object_put(obj);
    return ERROR;
}

/*Функция преобразует внутренний формат данных в формат json*/
void set_data_to_json_object(struct json_object *obj, char *name_key, char *value_obj)
{
    struct json_object *value;

    value = json_object_new_string(value_obj);
    json_object_object_add(obj, name_key, value);
}

/*Функция записывает форматированный данные в файл json*/
int write_data_to_json_file(char const *file_path)
{
    /* main_obj <- person_obj <- address_obj*/
    struct json_object *person_obj;
    struct json_object *address_obj;
    struct json_object *main_obj;
    main_obj = json_object_new_object();

    char age[NUMBER_LEN], home_num[NUMBER_LEN], index[NUMBER_LEN];
    int i = 0;

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    while(person != NULL)
    {
        address = person->address_point;

        person_obj = json_object_new_object();
        address_obj = json_object_new_object();

        i++;

        sprintf(age, "%d", person->age);             // Используемая далее функция json_object_object_add
        sprintf(home_num, "%d", address->home_num); // в качестве параметра - "значения ключа" принимает строку,
        sprintf(index, "%d", i);                   // а внутренний формат переменной в программе является числовым,
                                                  // поэтому возникла необходимость переводить числов в строку

        set_data_to_json_object(person_obj, "Name", person->name);
        set_data_to_json_object(person_obj, "Age", age);
        set_data_to_json_object(address_obj, "Street Name", address->street_name);
        set_data_to_json_object(address_obj, "Home Number", home_num);

        json_object_object_add(person_obj,"Address", address_obj);
        json_object_object_add(main_obj, index, person_obj);

        memset(age, 0, strlen(age));
        memset(home_num, 0, strlen(home_num));

        person = person->next;
    }

    if(json_object_to_file(file_path, main_obj) != VALID)
    {
        printf("\nATTENTION! Database not saved to file.\n");
        return ERROR;
    }

    json_object_put(main_obj);

    printf("Changes are saved to a file %s\n", file_path);
    change_flag = 0;

    return VALID;
}