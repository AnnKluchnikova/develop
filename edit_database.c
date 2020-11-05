#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FILE "./database.txt"

enum SPECIAL_LABEL {
    ERROR = -1,
    VALID,
    STRING,
    NUMBER
};

enum CMD {
    CMD_SHOW_CMD = 1,
    CMD_GET_ALL_LISTS,
    CMD_CLEAR_ALL_LISTS,
    CMD_ADD_LIST_ITEM,
    CMD_FIND_RECORD_BY_NAME,
    CMD_EXIT,
    CMD_GET_ADDRESS_LIST,
    CMD_DELETE_PERSON,
    CMD_DELETE_ADDRESS,
    CMD_SAVE_ADDRESS_IN_FILE,
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
    address_t *addrs_point;
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

/*Функция для считывания данных пользователя*/
int read_data(char *buf, int special_label)
{
    if(special_label == STRING)
        fgets(buf, 32, stdin);
    else
        fgets(buf, 8, stdin);

    if(strchr(buf, ',') != NULL)
        return ERROR;

    char *trigger = strchr(buf, '\n');

    if (trigger != NULL)
        *trigger = '\0';
    else
        return ERROR;

    if(buf[0] == '\0')
        return ERROR;

    return VALID;
}

/*Проверка на отсутствие повторений адреса*/
address_t *check_address_matches(char *street_name, int home_num)
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
    address_t *ret = check_address_matches(street_name, home_num);

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
void add_new_person(char *name, unsigned int age, char *street_name, int home_num)
{
    people_t *new_person = calloc(1,sizeof(people_t));

    list_of_people->item_counter++;
    new_person->name = strdup(name);
    new_person->age = age;
    new_person->addrs_point = add_new_address(street_name, home_num);

    if(list_of_people->head == NULL)
        list_of_people->head = new_person;
    else
    {
        list_of_people->tail->next = new_person;
        new_person->prev = list_of_people->tail;
        new_person->next = NULL;
    }
    
    list_of_people->tail = new_person;
}

/*Вывод списка имен на экран*/
void view_all_lists()
{
    if(list_of_people->head == NULL)
    {
        printf("ERROR! No records!\n");
        return;
    }

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    int index = 0;

    while(person != NULL)
    {
        printf("%d. Name: %s, Age: %u ",
            index,
            person->name,
            person->age);

        address = person->addrs_point;

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
    if(list_of_address->head == NULL)
    {
        printf("ERROR! No records!\n");
        return;
    }

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

/*Очистка списка адресов*/
void clear_list_of_address()
{
    if(list_of_address->head == NULL)
        return;

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
}

/*Очистка списка людей*/
void clear_list_of_people()
{
    if(list_of_people->head == NULL)
        return;

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
}

/*Полная очистка списка*/
void clear_all_lists()
{
    clear_list_of_people();
    clear_list_of_address();
}

/*Поиск элемента по частичному совпадению с именем*/
void search_record_by_name_pattern(char *name)
{
    people_t *person = list_of_people->head;
    address_t *address = NULL;

    int len = strlen(name);

    for(int index = 0; index < list_of_people->item_counter; index++)
    {
        if(strncmp(name, person->name, len) == 0)
        {
            printf("%d. Name: %s, Age: %u ",
                index,
                person->name,
                person->age);

            address = person->addrs_point;

            printf("Street: %s, House number: %d \n",
                address->street_name,
                address->home_num);
        }

        person = person->next;
    }
}

/*Поиск элемента*/
void search_record_by_name()
{
    if(list_of_people->head == NULL)
    {
        printf("ERROR! No records!\n");
        return;
    }

    char name[32];

    printf("\nEnter a name: ");
    if(read_data(name, STRING) != VALID)
        return;

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    int items_found = 0, index = 0;

    for(; index < list_of_people->item_counter; index++)
    {
        if(strcmp(person->name, name) == 0)
        {
            items_found++;
            printf("%d. Name: %s, Age: %u ",
                index,
                person->name,
                person->age);

            address = person->addrs_point;

            printf("Street: %s, House number: %d \n",
                address->street_name,
                address->home_num);
        }

        person = person->next;
    }

    if(items_found == 0)
        search_record_by_name_pattern(name);
}

/*Удаление элемента по имени, без затрагивания списка адресов*/
void delete_record_from_people_list()
{
    if(list_of_people->head == NULL)
    {
        printf("ERROR! No records!\n");
        return;
    }

    char name[32];

    printf("\nEnter a name: ");
    if(read_data(name, STRING) != VALID)
        return;

    people_t *delete_person = list_of_people->head;
    people_t *next_person = NULL;

    while(delete_person != NULL)
    {
        next_person = delete_person->next;

        if(strcmp(delete_person->name, name) == 0)
        {
            if((delete_person == list_of_people->head)&&(delete_person == list_of_people->tail))
            {
                clear_list_of_people();

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

            free(delete_person->name);
            free(delete_person);

            list_of_people->item_counter--;
        }

        delete_person = next_person;
    }
}

/*Проверка на отсутствие связей между адресом и людьми*/
int check_connect_btwn_address_person(address_t *address)
{
    people_t *person = list_of_people->head;

    int ret = 0;

    while(person != NULL)
    {
        if(person->addrs_point == address)
            ret++;

        person = person->next;
    }

    return ret;
}

int read_address_data (char *street_name, char *home_num)
{
    printf("\nEnter the person's address\n Street: ");
    if(read_data(street_name, STRING) != VALID) goto false_data;

    printf(" Нouse number: ");
    if(read_data(home_num, NUMBER) != VALID) goto false_data;

    return VALID;

false_data:
    printf("\nIncorrect data! Try again, please.\n");
    return ERROR;
}

/*Удаление элемента по имени из списка адресов*/
void delete_record_from_address_list()
{
    if(list_of_address->head == NULL)
    {
        printf("ERROR! No records!\n");
        return;
    }

    char street_name[32], home_num[8];

    if(read_address_data(street_name, home_num) != VALID)
        return;

    int ret = atoi(home_num);
    if(ret == 0)
    {
        printf("\nIncorrect data! Try again, please.\n");
        return;
    }

    address_t *address = check_address_matches(street_name, ret);

    if(address != NULL)
    {
        if(check_connect_btwn_address_person(address) != 0)
        {
            printf("\n ERROR! There are people with this address.\n");
            return;
        }
    }
    else
    {
        printf("\n ERROR! There is no such address!\n");
        return;
    }

    address_t *delete_address = list_of_address->head;

    while(delete_address != NULL)
    {
        if(delete_address == address)
        {
            if((delete_address == list_of_address->head)&&(delete_address == list_of_address->tail))
            {
                clear_list_of_address();

                break;
            }
            else if(delete_address == list_of_address->head)
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
}

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
/*Создать новую функцию проверки содержимого файла*/
    char str_from_file[128];
    char name[64], age[8], street_name[64], home_num[8];
    int ret_age = 0, ret_home_num = 0;

   /* while(fgets(str_from_file, 75, file) != NULL)
        strtok()*/

    while(fscanf(file, "%[a-zA-z ],%[^,],%[^,],%s\n", name, age, street_name, home_num) != EOF)
    {
        ret_age = atoi(age);
        if (ret_age != 0)
        {
            ret_home_num = atoi(home_num);
            if(ret_home_num != 0)
                add_new_person(name, ret_age, street_name, ret_home_num);
            else 
            {
                printf("\n ERROR! The database contains incorrect data.\n"
                    " Please try again later.\n");

                clear_all_lists();

                fclose(file);

                return ERROR;
            }
        }
    }
/**/
    fclose(file);

    return VALID;
}

/*Функция считывания данных пользователя*/
int read_user_data()
{
    char name[32], age[8], street_name[32], home_num[8];
    long int ret_age = 0, ret_home_num = 0;

    printf("\nEnter information about the person\n"
            " Name: ");
    if(read_data(name, STRING) != VALID) goto false_data;

    printf(" Age: ");
    if(read_data(age, NUMBER) != VALID) goto false_data;

    ret_age = atoi(age);
    if (ret_age == 0) goto false_data;

    if(read_address_data(street_name, home_num) != 0)
        return ERROR;

    ret_home_num = atoi(home_num);
    if(ret_home_num == 0) goto false_data;

    add_new_person(name, ret_age, street_name, ret_home_num);

    return VALID;

false_data:
    printf("\nIncorrect data! Try again, please.\n");
    return ERROR;
}

/*Функция сохранения изменений в файл*/
void save_to_file(char const *file_path)
{
    FILE *file;
    file = fopen(file_path,"w");

    if(file == NULL)
    {
        printf("ERROR! Failed to save!\n");
        return;
    }

    people_t *person = list_of_people->head;
    address_t *address = NULL;

    while(person != NULL)
    {
        address = person->addrs_point;
        fprintf(file, "%s,%d,%s,%d\n", person->name,
                                       person->age,
                                       address->street_name,
                                       address->home_num);

        person = person->next;
    }

    fclose(file);

    printf("Changes are saved to a file %s\n", file_path);
}

/*Проверка сохранения перед выходом из программы*/
int question_about_save(char const *file_path)
{
    char answer[4];

    printf("\n Save changes? yes/no\n");
    scanf("%s", answer);

    if(strcmp(answer,"yes") == 0)
        save_to_file(file_path);
    else if (strcmp(answer,"no") == 0)
        printf("The changes are not saved!\n");
    else
    {
        printf("Use 'yes' or 'no' for the response.\n");
        return ERROR;
    }

    return VALID;
}

/*Функция обработки команды*/
int processing_of_command_name()
{
    char user_message[32];

    if(read_data(user_message, STRING) != VALID)
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
    else if (strcmp(user_message,"delete address") == 0)
        return CMD_DELETE_ADDRESS;
    else if (strcmp(user_message,"save database") == 0)
        return CMD_SAVE_ADDRESS_IN_FILE;

    return ERROR;
}

int command_execution(int cmd_num, char const *file_path)
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
                   "delete address\n"
                   "save database\n"
                   "delete database\n"
                   "exit\n");
            break;
        case CMD_GET_ALL_LISTS: /*Вывод списка на экран*/
            view_all_lists();
            break;
        case CMD_CLEAR_ALL_LISTS: /*Полная очистка списка*/
            clear_all_lists();
            break;
        case CMD_ADD_LIST_ITEM: /*Добавление нового элемента*/
            read_user_data();
            break;
        case CMD_FIND_RECORD_BY_NAME: /*Поиск элемента*/
            search_record_by_name();
            break;
        case CMD_EXIT: /*Выход из программы*/
            if(question_about_save(file_path) == ERROR)
                cmd_num = ERROR;
            else
                clear_all_lists();
            break;
        case CMD_GET_ADDRESS_LIST: /*Вывод адресов*/
            view_list_of_address();
            break;
        case CMD_DELETE_PERSON: /*Удаление человека из списка*/
            delete_record_from_people_list();
            break;
        case CMD_DELETE_ADDRESS: /*Удаление адреса из списка*/
            delete_record_from_address_list();
            break;
        case CMD_SAVE_ADDRESS_IN_FILE:
            save_to_file(file_path);
            break;
        default:
            printf(" Invalid command! Try again, please.\n"
                "Enter the 'show cmd' command to view the command reference.\n");
            break;
    }

    __fpurge(stdin);
    return cmd_num;
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

    if(read_file(file_path) != 0) goto no_file;

        printf("\n*** Welcome to the people database editor! ***\n"
            " You are editing a file: %s\n"
            "\n Enter the 'show commands' command to view the command reference.", file_path);

        int cmd_num = 0;

        while(cmd_num != CMD_EXIT)
        {
            printf("\n >");
            cmd_num = processing_of_command_name();
            cmd_num = command_execution(cmd_num, file_path);
        }

no_file:
    free(list_of_people);
    free(list_of_address);

    return 0;
}
