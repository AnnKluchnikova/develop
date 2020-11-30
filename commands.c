#include <stdio.h>
#include <string.h> // для strcmp()
#include <signal.h> // для raise()

#include "setdata.h"
#include "listedit.h"
#include "types.h"
#include "getjson.h"

#define PRESSED_ENTER -2

/*Запрос о сохранении базы перед выходом из программы*/
void ask_question_about_save(char const *file_path)
{
    char answer[STRING_LEN];

    while(1)
    {
        printf("\n Save changes? yes/no\n");
        read_data(answer, STRING_LEN);

        if(strcmp(answer,"yes") == 0)
        {
            write_data_to_json_file(file_path);
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

/*Функция обработки команды, которая запрашивает имя команды 
и возвращает ее номер-макрос для выполнение*/
int process_command_name(void)
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
    else if (strcmp(user_message, "") == 0)
        return PRESSED_ENTER;

    return ERROR;
}

/*Функция выполнения команды, которая получает номер-макрос команды
и вызывает соответствующие функции*/
void execute_command(int cmd_num, char const *file_path)
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
            clear_all_lists(NEED_MESSAGE);
            break;
        case CMD_ADD_LIST_ITEM: /*Добавление нового элемента*/
            get_data_to_add();
            break;
        case CMD_FIND_RECORD_BY_NAME: /*Поиск элемента*/
            get_name_to_serch();
            break;
        case CMD_EXIT: /*Выход из программы*/
            if(change_flag > 0)
                ask_question_about_save(file_path);
            clear_all_lists(NO_MESSAGE);
            break;
        case CMD_GET_ADDRESS_LIST: /*Вывод адресов*/
            view_list_of_address();
            break;
        case CMD_DELETE_PERSON: /*Удаление человека из списка*/
            get_name_to_delete();
            break;
        case CMD_SAVE_DATABASE_IN_FILE:
            write_data_to_json_file(file_path);
            break;
        case PRESSED_ENTER:
            break;
        default:
            printf(" Invalid command! Try again, please.\n"
                   " Enter the 'show commands' command to view the command reference.\n");
            break;
    }
}
