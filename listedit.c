#include <stdio.h>
#include <stdlib.h>  // для free(), calloc()
#include <string.h> //для strcmp(), strdup(), strlen(), strncasecmp(), strtok()

#include "types.h"
#include "setdata.h"

#define FILE_STRING_LEN 256 // Длинна строки, считываемая из файла
#define NUM_FIRST_LINE 1 // В стандартных редакторах нумерация строк начинается с "1"

//________________________________________________________________________ДОБАВЛЕНИЕ ЭЛЕМЕНТА
/*Функция передачи указателя на уже существующий адрес*/
address_t *get_existing_address(const char *street_name, int home_num)
{
    address_t *address = list_of_address->head;

    while(address != NULL)
    {
        if((home_num == address->home_num)&&(strcmp(street_name, address->street_name) == 0))
            return address;

        address = address->next;
    }

    return NULL;
}

/*Добавление записи в справочник адресов*/
address_t *add_new_address(const char *street_name, int home_num)
{
    address_t *new_address = NULL;
    address_t *ret = get_existing_address(street_name, home_num);

    if(ret != NULL)
        return ret;

    new_address = calloc(1,sizeof(address_t));

    list_of_address->item_counter++;
    new_address->street_name = strdup(street_name);
    new_address->home_num = home_num; 

    if(list_of_address->head == NULL)
        list_of_address->head = new_address;
    else
    {
        list_of_address->tail->next = new_address;
        new_address->prev = list_of_address->tail;
        new_address->next = NULL;
    }

    list_of_address->tail = new_address;

    return new_address;
}

/*Добавление записи в список людей*/
void add_new_person(const char *name,
                    int age,
                    const char *street_name,
                    int home_num,
                    int show_message)
{
    people_t *new_person = calloc(1,sizeof(people_t));

    list_of_people->item_counter++;
    new_person->name = strdup(name);
    new_person->age = age;
    new_person->address_point = add_new_address(street_name, home_num);

    if(list_of_people->head == NULL)
        list_of_people->head = new_person;
    else
    {
        list_of_people->tail->next = new_person;
        new_person->prev = list_of_people->tail;
        new_person->next = NULL;
    }
    
    list_of_people->tail = new_person;

    if(show_message == NEED_MESSAGE)
        printf("\nReady! The element is inserted.\n");
}

/*Функция проверки полученных данных для последующего добавления*/
int add_correct_data_to_database(char *name,
                                 char *age,
                                 char *street_name,
                                 char *home_num,
                                 message show_message)
{
    int ret_home_num = 0, ret_age = 0;

    if(is_correct_string(name) == ERROR)
        return ERROR;

    ret_age = get_age_as_number(age);
    if (ret_age == ERROR)
        return ERROR;

    if(is_correct_string(street_name) == ERROR)
        return ERROR;

    ret_home_num = get_home_number_as_number(home_num);
    if(ret_home_num == ERROR)
        return ERROR;

    add_new_person(name, ret_age, street_name, ret_home_num, show_message);

    return VALID;
}

/*Функция считывания данных пользователя с консоли*/
int get_data_to_add(void)
{
    char name[STRING_LEN], age[NUMBER_LEN], street_name[STRING_LEN], home_num[NUMBER_LEN];

    printf("\nEnter information about the person\n"
            " Name (length from 1 to 63 characters without ','): ");
    if(read_data(name, STRING_LEN) == ERROR)
        return ERROR;

    printf(" Age (use only '0123456789', calculated in years): ");
    if(read_data(age, NUMBER_LEN) == ERROR)
        return ERROR;

    printf("\nEnter the person's address\n Street "
           "(length from 1 to 63 characters without ','): ");
    if(read_data(street_name, STRING_LEN) == ERROR)
        return ERROR;

    printf(" Нouse number (use only '0123456789', "
           "no more than seven characters): ");
    if(read_data(home_num, NUMBER_LEN) == ERROR)
        return ERROR;

    if(add_correct_data_to_database(name, age, street_name, home_num, NEED_MESSAGE) != VALID)
        return ERROR;

    change_flag++;

    return VALID;
}

//________________________________________________________________________ВЫЫВОД БАЗЫ НА ЭКРАН
/*Вывод списка имен на экран*/
void view_all_lists(void)
{
    int index = NUM_FIRST_LINE;

    people_t *person = list_of_people->head;;
    address_t *address = NULL;

    if(list_of_people->head == NULL)
    {
        printf("Attention! The database is empty.\n");
        return;
    }

    while(person != NULL)
    {
        printf("%d. Name: %s\tAge: %u\t",
            index,
            person->name,
            person->age);

        address = person->address_point;

        printf("Street: %s \tHouse number: %d \n",
            address->street_name,
            address->home_num);

        index++;

        person = person->next;
    }
}

/*Вывод списка адресов на экран*/
void view_list_of_address(void)
{
    int index = NUM_FIRST_LINE;
    address_t *address = list_of_address->head;;

    if(list_of_people->head == NULL)
    {
        printf("Attention! The database is empty.\n");
        return;
    }

    while(address != NULL)
    {

        printf("%d. Street: %s\tHouse number: %d \n",
            index,
            address->street_name,
            address->home_num);

        index++;

        address = address->next;
    }
}

//________________________________________________________________________ПОЛНАЯ ОЧИСТКА БАЗЫ
/*Очистка списка адресов*/
int clear_list_of_address(void)
{
    address_t *address = NULL;

    if(list_of_address->head == NULL)
        return ERROR;

    while(list_of_address->head != NULL)
    {
        address = list_of_address->head;
        free(list_of_address->head->street_name);

        list_of_address->head = address->next;
        free(address);
    }

    list_of_address->item_counter = 0;
    list_of_address->head = list_of_address->tail = NULL;

    return VALID;
}

/*Очистка списка людей*/
int clear_list_of_people(void)
{
    people_t *person = NULL;

    if(list_of_people->head == NULL)
        return ERROR;

    while(list_of_people->head != NULL)
    {
        person = list_of_people->head;
        free(list_of_people->head->name);

        list_of_people->head = person->next;
        free(person);
    }

    list_of_people->item_counter = 0;
    list_of_people->head = list_of_people->tail = NULL;

    return VALID;
}

/*Полная очистка списка*/
int clear_all_lists(message show_message)
{
    int ret_clear_people = -1, ret_clear_address = -1;

    ret_clear_people = clear_list_of_people();
    ret_clear_address = clear_list_of_address();

    /*Если предположить, что поля данных могут быть
    *пустые (в этой версии такое не предусмотренно),
    *например, может отсутствовать адрес, то стоит
    *считать базу очищенное, если очищен один из списков.
    */
    if((ret_clear_people == VALID)||(ret_clear_address == VALID))
    {
        if(show_message == NEED_MESSAGE)
            printf("\nReady! Database deleted.\n");

        change_flag++;

        return VALID;
    }

    if(show_message == NEED_MESSAGE)
        printf("\nAttention! There is nothing to remove. "
               "The database is empty.\n");
    return ERROR;
}

//________________________________________________________________________ПОИСК ЭЛЕМЕНТА
/*Поиск элемента по частичному совпадению с именем*/
int search_record_by_name_pattern(const char *name)
{
    int items_found = 0;
    int len = 0;

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    if(list_of_people->head == NULL)
    {
        printf("Attention! The database is empty.\n");
        return ERROR;
    }

    len = strlen(name);

    for(int index = NUM_FIRST_LINE; index <=list_of_people->item_counter; index++)
    {
        if(strncasecmp(name, person->name, len) == 0)
        {
            items_found++;

            printf("%d. Name: %s, Age: %u ",
                index,
                person->name,
                person->age);

            address = person->address_point;

            printf("Street: %s, House number: %d \n",
                address->street_name,
                address->home_num);
        }

        person = person->next;
    }

    return items_found;
}

/*Поиск элемента*/
void search_record_by_name(const char *name)
{
    int items_found = 0;

    people_t *person= list_of_people->head;
    address_t *address = NULL;

    if(list_of_people->head == NULL)
    {
        printf("Attention! The database is empty.\n");
        return;
    }

    for(int index = 0; index < list_of_people->item_counter; index++)
    {
        if(strcasecmp(person->name, name) == 0)
        {
            items_found++;

            printf("%d. Name: %s, Age: %u ",
                index,
                person->name,
                person->age);

            address = person->address_point;

            printf("Street: %s, House number: %d \n",
                address->street_name,
                address->home_num);
        }

        person = person->next;
    }

    if(items_found == 0)
        items_found =search_record_by_name_pattern(name);

    if(items_found == 0)
        printf("There is no such person!\n");
}

/*Получения корректного имени для дальнейшего поиска в базе*/
int get_name_to_serch(void)
{
    char name[STRING_LEN];

    printf("\nEnter a name "
           "(length from 1 to 63 characters without ','): ");
    if(read_data(name, STRING_LEN) == ERROR)
        goto false_data;
    if(is_correct_string(name) == ERROR)
        goto false_data;

    search_record_by_name(name);

    return VALID;

false_data:
    printf("\nIncorrect data! Try again, please.\n");
    return ERROR;
}

//________________________________________________________________________УДАЛЕНИЕ ЭЛЕМЕНТА
/*Вывод сообщений после работы функции удаления записи*/
int show_message_about_delete(int items_deleted)
{
    if(items_deleted != 0)
    {
        printf("\n Ready! The person's record deleted.\n");
        change_flag++;

        return VALID;
    }

    printf("\nERROR! There is no such person!\n");
    return ERROR;
}

/*Проверка на принадлежности адреса человеку*/
int search_address_belong_to_people(address_t *address)
{
    people_t *person = list_of_people->head;

    while(person != NULL)
    {
        if(person->address_point == address)
            return ERROR;

        person = person->next;
    }

    return VALID;
}

/*Удаление адреса из списка*/
int delete_address_record(address_t *address)
{
    address_t *delete_address = list_of_address->head;

    if(address == NULL)
        return ERROR;

    if(search_address_belong_to_people(address) != VALID)
        return ERROR;

    while(delete_address != NULL)
    {
        if(delete_address == address)
        {
            if(delete_address == list_of_address->head)
            {
                (delete_address->next)->prev = NULL;
                list_of_address->head = delete_address->next;
            }
            else if(delete_address == list_of_address->tail)
            {
                (delete_address->prev)->next = NULL;
                list_of_address->tail = delete_address->prev;
            }
            else
            {
                (delete_address->prev)->next = delete_address->next;
                (delete_address->next)->prev = delete_address->prev;
            }

            list_of_address->item_counter--;

            free(delete_address->street_name);
            free(delete_address);

            break;
        }

        delete_address = delete_address->next;
    }

    return VALID;
}

/*Удаление элемента по имени*/
int delete_person_record(const char *name)
{
    int items_deleted = 0;

    people_t *delete_person = list_of_people->head;
    people_t *next_person = NULL;
    address_t *delete_address = NULL;

    if(list_of_people->head == NULL)
    {
        printf("Attention! The database is empty.\n");
        return ERROR;
    }

    while(delete_person != NULL)
    {
        next_person = delete_person->next;

        if(strcmp(delete_person->name, name) == 0)
        {
            if((delete_person == list_of_people->head)&&(delete_person == list_of_people->tail))
            {
                items_deleted++;

                clear_all_lists(NEED_MESSAGE);
                break;
            }
            else if(delete_person == list_of_people->head)
            {
                (delete_person->next)->prev = NULL;
                list_of_people->head = delete_person->next;
            }
            else if(delete_person == list_of_people->tail)
            {
                (delete_person->prev)->next = NULL;
                list_of_people->tail = delete_person->prev;
            }
            else
            {
                (delete_person->prev)->next = delete_person->next;
                (delete_person->next)->prev = delete_person->prev;
            }

            delete_address = delete_person->address_point;

            free(delete_person->name);
            free(delete_person);

            delete_address_record(delete_address);

            list_of_people->item_counter--;
            items_deleted++;
        }

        delete_person = next_person;
    }

    return show_message_about_delete(items_deleted);
}

/*Получение корректного имени для последующего удаления записи*/
int get_name_to_delete(void)
{
    char name[STRING_LEN];

    printf("\nEnter a name: ");
    if(read_data(name, STRING_LEN) == ERROR)
        goto false_data;
    if(is_correct_string(name) == ERROR)
        goto false_data;

    delete_person_record(name);

    return VALID;

false_data:
    printf("\nIncorrect data! Try again, please.\n");
    return ERROR;
}

//________________________________________________________________________РАБОТА С ФАЙЛОМ
/*
*Функции read_file и save_to_file предназначены для работы с текстовыми файлами.
*Однако функции работают со своим форматом представления данных. Корректными будут
*считаться только не пустые строки в файле типа:
*
*012345678901234567890123456789012345678901234567890123456789012,149,012345678901234567890123456789012345678901234567890123456789012,1234567
*Ann Kluchnikova,21,O.Koshevoy,27
*,wret,56,sety,45                 // будет считано wret 56 sety 45
*Пустой файл
*
*Такие данные функция read_file может считать и записать во внутреннюю структуру программы.
При этом функци save_to_file будет сохранять данные из внутренней структуры в виде:
"Ann Kluchnikova,21,O.Koshevoy,27" (без кавычек), где запятая является разделителем
*/

/*Функция проверки доступности файла и проверки/записи его содержимого*/
/*int read_file(char const *file_path)
{
    char file_string[FILE_STRING_LEN]
    char *name, *age, *street_name, *home_num;
    FILE *file;
    int string_counter = 0;

    file = fopen(file_path, "r");
    if(file == NULL)
    {
        printf("\n Сouldn't open the file!\n"
               " Please try again later.\n");
        return ERROR;
    }

    while(fgets(file_string, FILE_STRING_LEN, file) != NULL)
    {
        string_counter++;

        name = strtok(file_string, ",");
        age = strtok(NULL, ",");
        street_name = strtok(NULL, ",");
        home_num = strtok(NULL, "\n");

        if(add_correct_data_to_database(name, age, street_name, home_num, NO_MESSAGE) != VALID)
        {
            printf("\n Invalid data in line: %d\n"
                   " Please correct the data and try again.\n", string_counter);

            clear_all_lists(NO_MESSAGE);
            fclose(file);

            return ERROR;
        }
    }

    fclose(file);

    printf("\n Ready! %d lines of data received.\n", string_counter);

    return VALID;
}*/

/*Функция сохранения изменений в файл*/
/*int save_to_file(char const *file_path)
{
    FILE *file = fopen(file_path,"w");

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    if(file == NULL)
    {
        printf("ERROR! Failed to save!\n");
        return ERROR;
    }

    while(person != NULL)
    {
        address = person->address_point;
        fprintf(file, "%s,%d,%s,%d\n", person->name,
                                       person->age,
                                       address->street_name,
                                       address->home_num);

        person = person->next;
    }

    fclose(file);

    printf("Changes are saved to a file %s\n", file_path);
    change_flag = 0;

    return VALID;
}*/