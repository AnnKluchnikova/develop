#include <stdio.h>
#include <json.h>
#include <string.h> // для strcpy(), для strcmp()
#include <stdlib.h> // для calloc()

#include "types.h"
#include "listedit.h"

#define ADDRESS_FIELDS_NUMBER 2 // streetName, homeNumber
#define PERSON_FIELDS_NUMBER 3 // name, age, address

/*
*Нужно использовать формат json для твоей базы данных.
*Т.е. тебе нужно использовать стороннюю библиотеку для парсинга 
*и сохранения файла. Возможно потребуется изменение структуры списка.
*/

/*Функция записывает значения объекта json в массив.
В конце выводит на экран связку "ключ:значение".
Так как функция будет вызываться в самом начале работы программы, вывод полученных
данных будет удобен для пользователя. Он сможет сразу проанализировать полученную
таблицу или увидеть на чтении какой строки произошла ошибка.*/
int write_value_to_array(struct json_object_iterator* itr,
                         char *array_to_write)
{
    char value[STRING_LEN];
    struct json_object *obj_value = json_object_iter_peek_value(itr);

    if(obj_value == NULL)
    {
        printf("ERROR! An empty value was encountered.");
        return ERROR;
    }

    strcpy(value, json_object_get_string(obj_value));
    strcpy(array_to_write, value);

    printf("%s:\t%s\t", json_object_iter_peek_name(itr), value);

    return VALID;
}

/*Функция для получения информации об адресе, в частности название улицы и 
номер дома, которые будут записаны в соответствующие массивы*/
int get_address_information(struct json_object_iterator *itr, char *street_name, char *home_num)
{
    char key[STRING_LEN];
    int obj_obj_obj_len = 0;

    struct json_object_iterator *loc_itr;

    struct json_object *obj_obj_obj = json_object_iter_peek_value(itr);
    obj_obj_obj_len = json_object_object_length(obj_obj_obj);

    if(obj_obj_obj_len > ADDRESS_FIELDS_NUMBER)
    {
        printf("ERROR! There are more entries in the address data than expected.\n");
        return ERROR;
    }

    loc_itr = calloc(1, sizeof(struct json_object_iterator));
    *loc_itr = json_object_iter_begin(obj_obj_obj);

    for(int i = 0; i < obj_obj_obj_len; i++)
    {
        strcpy(key, json_object_iter_peek_name(loc_itr));

        if(strcmp(key, "Street Name") == 0)
            write_value_to_array(loc_itr, street_name);
        else if(strcmp(key, "Home Number") == 0)
            write_value_to_array(loc_itr, home_num);
        else
        {
            printf("ERROR! Unexpected key encountered.\n");

            free(loc_itr);
            return ERROR;
        }

        json_object_iter_next(loc_itr);
    }

    free(loc_itr);
    return VALID;
}

/*Функция записывает в память(массивы) отдельные значения из объекта json.
Предполагается, что дальше их будут передавать в функцию добавления элемента*/
int get_value_from_json_object(struct json_object *obj_obj,
                               char *name, char *age,
                               char *street_name, char *home_num)
{
    char key[STRING_LEN];
    int obj_obj_len = 0;
    struct json_object_iterator *itr;

    obj_obj_len = json_object_object_length(obj_obj);

    if(obj_obj_len > PERSON_FIELDS_NUMBER)
    {
        printf("ERROR! There are more entries in the person's data than expected.\n");
        return ERROR;
    }

    itr = calloc(1, sizeof(struct json_object_iterator));
    *itr = json_object_iter_begin(obj_obj);

    /*Чтобы избавиться от жесткой привязки,
    *нужно проходить по циклу с проверкой значения ключа:
    *какое значение, такой и вызов функции записи*/
    for(int i = 0; i < obj_obj_len; i++)
    {
        strcpy(key, json_object_iter_peek_name(itr));

        if(strcmp(key, "Name") == 0)
            write_value_to_array(itr, name);
        else if(strcmp(key, "Age") == 0)
            write_value_to_array(itr, age);
        else if(strcmp(key, "Address") == 0)
            get_address_information(itr, street_name, home_num);
        else
        {
            printf("ERROR! Unexpected key encountered.\n");

            free(itr);
            return ERROR;
        }

        json_object_iter_next(itr);
    }

    free(itr);
    return VALID;
}

/**/
int get_data_from_json_object(struct json_object_iterator *itr,
                       char *name, char *age,
                       char *street_name, char *home_num)
{
    int obj_obj_len = 0;
    struct json_object *obj_obj;

    obj_obj = json_object_iter_peek_value(itr);
    obj_obj_len = json_object_object_length(obj_obj);

    if(obj_obj_len == 0)     // TODO Если объект пуст, то пропускаем его?
        return VALID;

    printf("Object %s:\t", json_object_iter_peek_name(itr));

    if(get_value_from_json_object(obj_obj, name, age,
                                          street_name, home_num) != VALID)
    {
        return ERROR;
    }

    printf("\n");
    return VALID;
}

/*Функция очищает массивы от предыдущих записей*/
void delete_arrays_values(char *name, char *age, char *street_name, char *home_num)
{
    memset(name, 0, name[0]);
    memset(age, 0, age[0]);
    memset(street_name, 0, street_name[0]);
    memset(home_num, 0, home_num[0]);
}


int add_json_object_to_database(struct json_object *obj)
{
    char name[STRING_LEN], age[NUMBER_LEN], street_name[STRING_LEN], home_num[NUMBER_LEN];
    int obj_len = json_object_object_length(obj); // Определим количество объектов,
                                                 // т.е. кол-во людей в списке
    struct json_object_iterator *itr;

    if(obj_len == 0)
    {
        printf("\nATTENTION! There is no data in the file.\n");
        return VALID;
    }

    itr = calloc(1, sizeof(struct json_object_iterator));
    *itr = json_object_iter_begin(obj);

    for(int index = 0; index < obj_len; index++)
    {
        if(get_data_from_json_object(itr, name, age, street_name, home_num) != VALID)
            goto false_data;

        if(add_correct_data_to_database(name, age, street_name, home_num,
                                        NO_MESSAGE) != VALID)
            goto false_data;

        delete_arrays_values(name, age, street_name, home_num);

        json_object_iter_next(itr);
    }

    free(itr);
    return VALID;

false_data:
    free(itr);
    clear_all_lists(NO_MESSAGE);
    return ERROR;
}

/*Функция считывает данные файла json и записывает их во внутреннюю структуру*/
int read_json_file(char const *file_path)
{
    struct json_object *obj;
    obj = json_object_from_file(file_path); // TODO Функция не дает ответа пуст ли файл,
                                           // или ошибка в синтаксисе. Единым решением будет
                                          // просто завершить программу с рекомендациями.
    if(obj == NULL)
    {
        printf("ERROR! Check the file \"%s\" and try again.\n\n"
               "[HELP] If the file is empty, "
               "just insert '{}' into the file yourself.\n"
               "If the file has data, check its spelling for json compliance.", file_path);
        return ERROR;
    }

    if(add_json_object_to_database(obj) != VALID)
    {
        json_object_put(obj);
        return ERROR;
    }

    json_object_put(obj);
    return VALID;
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
    //main_obj <- person_obj <- address_obj
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