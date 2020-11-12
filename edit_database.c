#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "f_data_entry.h"
#include "f_database.h"
#include "f_database_file.h"
#include "types.h"

#define DEFAULT_FILE "./database.txt"

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

    change_flag = 0;
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
