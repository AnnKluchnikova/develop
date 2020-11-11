#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEFAULT_FILE "./database.txt"
#define STRING_LEN 64
#define NUMBER_LEN 8

int change_flag = 0; // Счетчик изменений для запроса сохранения в файл,
                       // если произошли какие-то изменения устанавливается флаг
enum EXIT_FUNC {
    ERROR = -1,
    VALID
};

enum CALL {        // Если вызов функции произошел с консоли,
    FILE_CALL,    // то по завршению ее выполнения  в консоле
    CONSOLE_CALL // будет выведено соответсвующее сообщение,
};              // а если из файла, то сообщений от функций не будет

enum CMD {
    CMD_SHOW_CMD = 1,
    CMD_GET_ALL_LISTS,
    CMD_CLEAR_ALL_LISTS,
    CMD_ADD_LIST_ITEM,
    CMD_FIND_RECORD_BY_NAME,
    CMD_EXIT,
    CMD_GET_ADDRESS_LIST,
    CMD_DELETE_PERSON,
    CMD_SAVE_DATABASE_IN_FILE,
};

typedef struct address_s
{
    char *street_name;
    int home_num;
    struct address_s *next;
    struct address_s *prev;
}address_t;

typedef struct list_address_s
{
    int item_counter;
    address_t *head;
    address_t *tail;
}list_address_t;

typedef struct people_s
{
    char *name;
    unsigned int age;
    address_t *address_point;
    struct people_s *next;
    struct people_s *prev;
} people_t;

typedef struct list_people_s
{
    int item_counter;
    people_t *head;
    people_t *tail;
}list_people_t;

list_people_t *list_of_people;
list_address_t *list_of_address;

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

//________________________________________________________________________РАБОТА С БАЗОЙ
/*Проверка базы на непустоту*/
int not_empty_database()
{
    if(list_of_people->head == NULL)
    {
        printf("Attention! The database is empty.\n");
        return ERROR;
    }

    return VALID;
}

//________________________________________________________________________ДОБАВЛЕНИЕ ЭЛЕМЕНТА
/*Функция передачи указателя на уже существующий адрес*/
address_t *get_existing_address(char *street_name, int home_num)
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
address_t *add_new_address(char *street_name, int home_num)
{
    address_t *ret = get_existing_address(street_name, home_num);

    if(ret != NULL)
        return ret;

    address_t *new_address = calloc(1,sizeof(address_t));

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
void add_new_person(char *name, unsigned int age, char *street_name, int home_num, int special_label)
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

    if(special_label == CONSOLE_CALL)
        printf("\nReady! The element is inserted.\n");
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

/*Функция считывания данных пользователя с консоли*/
int get_data_to_add()
{
    char name[STRING_LEN], age[NUMBER_LEN], street_name[STRING_LEN], home_num[NUMBER_LEN];

    printf("\nEnter information about the person\n"
            " Name (length from 1 to 63 characters without ','): ");
    if(read_data(name, STRING_LEN) == ERROR)
        goto false_data;

    printf(" Age (use only '0123456789', calculated in years): ");
    if(read_data(age, NUMBER_LEN) == ERROR)
        goto false_data;

    printf("\nEnter the person's address\n Street "
           "(length from 1 to 63 characters without ','): ");
    if(read_data(street_name, STRING_LEN) == ERROR)
        goto false_data;

    printf(" Нouse number (use only '0123456789', "
           "no more than seven characters): ");
    if(read_data(home_num, NUMBER_LEN) == ERROR)
        goto false_data;

    if(add_correct_data_to_database(name, age, street_name, home_num, CONSOLE_CALL) != VALID)
        goto false_data;

    change_flag++;

    return VALID;

false_data:
    //printf("\nIncorrect data! Try again, please.\n");
    return ERROR;
}

//________________________________________________________________________ВЫЫВОД БАЗЫ НА ЭКРАН
/*Вывод списка имен на экран*/
void view_all_lists()
{
    if(not_empty_database() == ERROR)
        return;

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    int index = 0;

    while(person != NULL)
    {
        printf("%d. Name: %s, Age: %u ",
            index,
            person->name,
            person->age);

        address = person->address_point;

        printf("Street: %s, House number: %d \n",
            address->street_name,
            address->home_num);

        index++;

        person = person->next;
    }
}

/*Вывод списка адресов на экран*/
void view_list_of_address()
{
    if(not_empty_database() == ERROR)
        return;

    address_t *address = list_of_address->head;

    int index = 0;

    while(address != NULL)
    {

        printf("%d. Street: %s, House number: %d \n",
            index,
            address->street_name,
            address->home_num);

        index++;

        address = address->next;
    }
}

//________________________________________________________________________ПОЛНАЯ ОЧИСТКА БАЗЫ
/*Очистка списка адресов*/
int clear_list_of_address()
{
    if(list_of_address->head == NULL)
        return ERROR;

    address_t *address = NULL;

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
int clear_list_of_people()
{
    if(list_of_people->head == NULL)
        return ERROR;

    people_t *person = NULL;

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
int clear_all_lists(int call)
{
    int ret_clear_people = -1, ret_clear_address = -1;

    ret_clear_people = clear_list_of_people();
    ret_clear_address = clear_list_of_address();

    if((ret_clear_people == VALID)&&(ret_clear_address == VALID))
    {
        if(call == CONSOLE_CALL)
            printf("\nReady! Database deleted.\n");

        change_flag++;

        return VALID;
    }

    if(call == CONSOLE_CALL)
        printf("\nAttention! There is nothing to remove. "
               "The database is empty.\n");
    return ERROR;
}

//________________________________________________________________________ПОИСК ЭЛЕМЕНТА
/*Поиск элемента по частичному совпадению с именем*/
int search_record_by_name_pattern(char *name)
{
    int items_found = 0;

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    int len = strlen(name);

    for(int index = 0; index < list_of_people->item_counter; index++)
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
void search_record_by_name(char *name)
{
    if(not_empty_database() == ERROR)
        return;

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    int items_found = 0;

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
int get_name_to_serch()
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
int message_about_delete(int items_deleted)
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
int address_belong_to_people(address_t *address)
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
    if(address != NULL)
    {
        if(address_belong_to_people(address) != VALID)
            return ERROR;
    }
    else
        return ERROR;

    address_t *delete_address = list_of_address->head;

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
int delete_person_record(char *name)
{
    if(not_empty_database() == ERROR)
        return ERROR;

    people_t *delete_person = list_of_people->head;
    people_t *next_person = NULL;
    address_t *delete_address = NULL;

    int items_deleted = 0;

    while(delete_person != NULL)
    {
        next_person = delete_person->next;

        if(strcmp(delete_person->name, name) == 0)
        {
            if((delete_person == list_of_people->head)&&(delete_person == list_of_people->tail))
            {
                items_deleted++;

                clear_all_lists(CONSOLE_CALL);
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

    return message_about_delete(items_deleted);
}

/*Получение корректного имени для последующего удаления записи*/
int get_name_to_delete()
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
/*Функция проверки доступности файла и проверки его содержимого*/
int read_file(char const *file_path)
{
    FILE *file;

    file = fopen(file_path, "r");
    if(file == NULL)
    {
        printf("\n Сouldn't open the file!\n"
               " Please try again later.\n");
        return ERROR;
    }

    char str_from_file[256], *name, *age, *street_name, *home_num;

    while(fgets(str_from_file, 145, file) != NULL)
    {
        name = strtok(str_from_file, ",");
        age = strtok(NULL, ",");
        street_name = strtok(NULL, ",");
        home_num = strtok(NULL, "\n");

        if(add_correct_data_to_database(name, age, street_name, home_num, FILE_CALL) != VALID)
            goto false_data;
    }

    fclose(file);
    return VALID;

false_data:
    printf("\n The database contains incorrect data.\n"
           " Please try again later.\n");

    clear_all_lists(FILE_CALL);
    fclose(file);

    return ERROR;
}

/*Функция сохранения изменений в файл*/
int save_to_file(char const *file_path)
{
    FILE *file;
    file = fopen(file_path,"w");

    if(file == NULL)
    {
        printf("ERROR! Failed to save!\n");
        return ERROR;
    }

    people_t *person = list_of_people->head;
    address_t *address = NULL;

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
}

//________________________________________________________________________РАБОТА С ПОЛЬЗОВАТЕЛЕМ
/*Запрос о сохранении базы перед выходом из программы*/
void question_about_save(char const *file_path)
{
    char answer[STRING_LEN];

    while(1)
    {
        printf("\n Save changes? yes/no\n");
        read_data(answer, STRING_LEN);

        if(strcmp(answer,"yes") == 0)
        {
            save_to_file(file_path);
            break;
        }
        else if (strcmp(answer,"no") == 0)
        {
            printf("The changes are not saved!\n");
            break;
        }
        else
            printf("Use 'yes' or 'no' for the response.\n");
    }
}

/*Функция обработки команды*/
int processing_of_command_name()
{
    char user_message[STRING_LEN];

    if(read_data(user_message, STRING_LEN) != VALID)
        return ERROR;

    if(strcmp(user_message,"show commands") == 0)
        return CMD_SHOW_CMD;
    else if (strcmp(user_message,"get persons") == 0)
        return CMD_GET_ALL_LISTS;
    else if (strcmp(user_message,"delete database") == 0)
        return CMD_CLEAR_ALL_LISTS;
    else if (strcmp(user_message,"add person") == 0)
        return CMD_ADD_LIST_ITEM;
    else if (strcmp(user_message,"find person") == 0)
        return CMD_FIND_RECORD_BY_NAME;
    else if (strcmp(user_message,"exit") == 0)
        return CMD_EXIT;
    else if (strcmp(user_message,"get address") == 0)
        return CMD_GET_ADDRESS_LIST;
    else if (strcmp(user_message,"delete person") == 0)
        return CMD_DELETE_PERSON;
    else if (strcmp(user_message,"save database") == 0)
        return CMD_SAVE_DATABASE_IN_FILE;

    return ERROR;
}

void command_execution(int cmd_num, char const *file_path)
{
    switch(cmd_num)
    {
        case CMD_SHOW_CMD: /*Вывод справочника команд*/
            printf("\nshow commands\n"
                   "add person\n"
                   "find person\n"
                   "get persons\n"
                   "delete person\n"
                   "get address\n"
                   "save database\n"
                   "delete database\n"
                   "exit\n");
            break;
        case CMD_GET_ALL_LISTS: /*Вывод списка на экран*/
            view_all_lists();
            break;
        case CMD_CLEAR_ALL_LISTS: /*Полная очистка списка*/
            clear_all_lists(CONSOLE_CALL);
            break;
        case CMD_ADD_LIST_ITEM: /*Добавление нового элемента*/
            get_data_to_add();
            break;
        case CMD_FIND_RECORD_BY_NAME: /*Поиск элемента*/
            get_name_to_serch();
            break;
        case CMD_EXIT: /*Выход из программы*/
            if(change_flag > 0)
                question_about_save(file_path);
            clear_all_lists(FILE_CALL);
            break;
        case CMD_GET_ADDRESS_LIST: /*Вывод адресов*/
            view_list_of_address();
            break;
        case CMD_DELETE_PERSON: /*Удаление человека из списка*/
            get_name_to_delete();
            break;
        case CMD_SAVE_DATABASE_IN_FILE:
            save_to_file(file_path);
            break;
        default:
            printf(" Invalid command! Try again, please.\n"
                   " Enter the 'show commands' command to view the command reference.\n");
            break;
    }
}

/*Головная фунция*/
int main(int argc, char const *argv[])
{
    char const *file_path;

    if(argc == 2)
        file_path = argv[1];
    else
        file_path = DEFAULT_FILE;

    list_of_people = calloc(1,sizeof(list_people_t));
    list_of_address = calloc(1,sizeof(list_address_t));

    if(read_file(file_path) != VALID)
        goto no_file;

    printf("\n*** Welcome to the people database editor! ***\n"
           " You are editing a file: %s\n"
           "\n Enter the 'show commands' command to view the command reference.", file_path);

    int cmd_num = 0;

    while(cmd_num != CMD_EXIT)
    {
        printf("\n >");
        cmd_num = processing_of_command_name();
        command_execution(cmd_num, file_path);
    }

no_file:
    free(list_of_people);
    free(list_of_address);

    return 0;
}
