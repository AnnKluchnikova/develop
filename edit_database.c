#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_FILE "./people.txt"

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
    if(list_of_people->head == NULL)
    {
        printf("ERROR! No records!\n");
        return;
    }

    clear_list_of_people();
    clear_list_of_address();

    printf("Ready! Database cleared.\n");
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
void search_record_by_name(char *name)
{
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
void delete_record_from_people_list(char *name)
{
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

    printf("\n Ready! The person's record is removed.\n");
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

/*Удаление элемента по имени из списка адресов*/
void delete_record_from_address_list(char *street_name, int home_num)
{
    address_t *address = check_address_matches(street_name, home_num);

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

            free(delete_address->street_name);
            free(delete_address);

            list_of_address->item_counter--;

            break;
        }
        delete_address = delete_address->next;
    }

    printf("\n Ready! The address is removed.\n");
}

/*Функция добавления в файл*/
FILE *save_address_to_file(char *file_path, FILE *file)
{
    address_t *address = list_of_address->head;

    while(address != NULL)
    {
        fprintf(file, "%s,%d\n", address->street_name, address->home_num);
        address = address->next;
    }
    printf("Addresses are saved to a file %s\n", file_path);
}

/*Функция обработки команды*/
int processing_of_command_name()
{
    char user_message[32];
    memset(user_message, ' ', 5);

    printf("\n >");
    scanf("%[a-z ]", user_message);
    __fpurge(stdin);

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
    /*else if (strcmp(user_message,"import address") == 0)
        return CMD_IMPORT_ADDRESS_FROM_FILE;*/
    else if (strcmp(user_message,"save address") == 0)
        return CMD_SAVE_ADDRESS_IN_FILE;

    return 0;
}

char *command_execution(int cmd_num, char *file_path)
{
    char user_message[8], name[32], street_name[32];
    int ret_age = 0, ret_home_num = 0;

    switch(cmd_num)
    {
        case CMD_SHOW_CMD: /*Вывод справочника команд*/
        {
            printf("\nshow commands\n"
                "add person\n"
                "find person\n"
                "get persons\n"
                "delete person\n"
                "get address\n"
                "delete address\n"
//                "import address\n"
                "save address\n"
                "delete database\n"
                "exit\n");
            break;
        }

        case CMD_GET_ALL_LISTS: /*Вывод списка на экран*/
        {
            view_all_lists();
            break;
        }

        case CMD_CLEAR_ALL_LISTS: /*Полная очистка списка*/
        {
            clear_all_lists();
            break;
        }

        case CMD_ADD_LIST_ITEM: /*Добавление нового элемента*/
        {
            printf("\nEnter information about the person\n"
                " Name: ");
            scanf("%s", name);
            printf(" Age: ");
            scanf("%s", user_message);

            ret_age = atoi(user_message);
            if (ret_age != 0)
            {
                printf("\nEnter the person's address\n Street: ");
                scanf("%s", street_name);
                printf(" Нouse number: ");
                scanf("%s", user_message);

                ret_home_num = atoi(user_message);
                if(ret_home_num != 0)
                    add_new_person(name, ret_age, street_name, ret_home_num);
                else
                    printf("\nIncorrect data! Try again, please.\n");
            }
            else
                printf("\nIncorrect data! Try again, please.\n");

            break;
        }

        case CMD_FIND_RECORD_BY_NAME: /*Поиск элемента*/
        {
            if(list_of_people->head == NULL)
            {
                printf("ERROR! No records!\n");
            }
            else
            {
                printf("\nEnter a name: ");
                scanf("%s",name);

                search_record_by_name(name);
            }

            break;
        }

        case CMD_EXIT: /*Выход из программы*/
        {
            if(list_of_people->head != NULL)
                clear_list_of_people();
            if(list_of_address->head != NULL)
                clear_list_of_address();

            break;
        }

        case CMD_GET_ADDRESS_LIST: /*Вывод адресов*/
        {
            view_list_of_address();
            break;
        }

        case CMD_DELETE_PERSON: /*Удаление человека из списка*/
        {
            if(list_of_people->head != NULL)
            {
                printf("\nEnter a name: ");
                scanf("%s", name);

                delete_record_from_people_list(name);
            }
            else
                printf("No element!\n");

            break;
        }

        case CMD_DELETE_ADDRESS: /*Удаление адреса из списка*/
        {
            if(list_of_address->head != NULL)
            {
                printf("\nEnter a street name: ");
                scanf("%s", street_name);
                printf("Enter a house number: ");
                scanf("%s", user_message);

                ret_home_num = atoi(user_message);
                if(ret_home_num != 0)
                    delete_record_from_address_list(street_name, ret_home_num);
                else
                    printf("\nIncorrect data! Try again, please.\n");
            }
            else
                printf("ERROR! No records!\n");

            break;
        }

/*        case CMD_IMPORT_ADDRESS_FROM_FILE:
        {
            char home_num[8];

            file =fopen(DEFAULT_ADDRESS_FILE, "r");
            while(file == NULL)
            {
                printf("\nEnter the file name and path:\n");
                scanf("%s", file_path);

                file = fopen(file_path, "r");
            }

            while(fscanf(file, "%[^,],%s\n", street_name, home_num) != EOF)
            {
                ret_home_num = atoi(home_num);
                if(ret_home_num != 0)
                    add_new_address(street_name, ret_home_num);
                else 
                {
                    printf("ERROR! The database contains incorrect data.\n");
                    clear_list_of_address();
                    break;
                }
            }

            fclose(file);
            __fpurge(stdin);

            break;
        }*/

        case CMD_SAVE_ADDRESS_IN_FILE:
        {
            FILE *file;

            printf("Save changes to the current file? y/n\n");
            scanf("%s", user_message);

            if(strcmp(user_message,"y") == 0)
            {
                file = fopen(file_path,"w");
                if(file != NULL)
                {
                    save_address_to_file(file_path, file);
                    fclose(file);
                }
            }
            else if (strcmp(user_message,"n") == 0)
            {
                char new_file_path[128];

                printf("Enter the full name of the new file:\n");
                scanf("%s", new_file_path);

                file = fopen(new_file_path,"a");
                if(file != NULL)
                {
                    save_address_to_file(new_file_path, file);
                    fclose(file);

                    file_path = strdup(new_file_path);
                }
            }
            else
            {
                printf("Invalid command! Try again, please.\n");
                break;
            }

            break;
        }

        /*case CMD_IMPORT_DATABASE_FROM_FILE:
        {
            while(fscanf(file, "%[a-zA-z ],%[^,],%[^,],%s\n", street_name, home_num) != EOF)
                printf("Name: %s, Age: %d, Street: %s, House number: %d\n",
                    name,
                    ret_age,
                    street_name,
                    ret_home_num);
        }*/

        default:
        {
            printf(" Invalid command! Try again, please.\n"
                "Enter the 'show cmd' command to view the command reference.\n");
            break;
        }
    }

    __fpurge(stdin);
    return file_path;
}

/*Головная фунция*/
int main(int argc, char const *argv[])
{
    FILE *file;
    char const *file_path;

    if(argc == 2)
        file_path = argv[1];
    else
        file_path = DEFAULT_FILE;

    list_of_people = calloc(1,sizeof(list_people_t));
    list_of_address = calloc(1,sizeof(list_address_t));

/*Сделать отдельную функцию*/
    file = fopen(file_path, "r");
    if(file == NULL)
    {
        printf(" Сouldn't open the file!\n"
            " Please try again another time.\n");

        return 0;
    }

    char street_name[32], home_num[8];
    int ret_home_num = 0;

    while(fscanf(file, "%[^,],%s\n", street_name, home_num) != EOF)
    {
        ret_home_num = atoi(home_num);
        if(ret_home_num != 0)
            add_new_address(street_name, ret_home_num);
        else 
        {
            printf("ERROR! The database contains incorrect data.\n"
                "Please try again another time!\n");

            clear_list_of_address();

            fclose(file);
            return 0;
        }
    }

    fclose(file);
/**/

    printf("*** Welcome to the people database editor! ***\n");

    int cmd_num = 0;

    printf(" Enter the 'show commands' command to view the command reference.");
    /*Команды*/
    while(cmd_num != CMD_EXIT)
    {
        printf("\n You are editing a file: %s", file_path);
        cmd_num = processing_of_command_name();
        path = command_execution(cmd_num, file_path);

        file_path = path;
    }

    free(list_of_people);
    free(list_of_address);

    return 0;
}
