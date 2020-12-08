#include <stdio.h>
#include <stdlib.h> // для free(), calloc()
#include <signal.h> // для signal(), для raise()

#include "commands.h"
#include "types.h"
#include "getjson.h"

#define DEFAULT_FILE "./database.json"

unsigned volatile char exit_signal_flag = 0; // Флаг выхода из программы по сигналу Ctrl+C
char const *file_path;               // Путь до файла, с данными которого будет работать программа

/*Функция обработки сигналов*/
void signal_handler(int signo)
{
    if(signo == SIGINT)
    {
        execute_command(CMD_EXIT, file_path);
        exit_signal_flag = 1;
    }
    else if(signo == SIGUSR1)
        printf("Received SIGUSR1!\n");
    else if(signo == SIGUSR2)
        printf("Received SIGUSR2!\n");
}

/*Функция проверки доступности файла на чтение и запись.
Пользователю должны быть доступны все функции программы,
в частности редактирование данных и сохранение их в файл.
Для этого необходимо иметь доступ к файлу для чтения его
данных и записи в него новых*/
int check_file_access(char const *file_path)
{
    FILE *file;

    file = fopen(file_path, "r+");
    if(file == NULL)
    {
        printf("\n The file \"%s\" is not open!\n"
               " Please сheck access rights.\n", file_path);
        return ERROR;
    }

    fclose(file);

    return VALID;
}

/*Головная фунция, которая может принять и обработать путь до файла при запуске программы*/
int main(int argc, char const *argv[])
{
    int cmd_num = 0;

    if(argc == 2)
        file_path = argv[1];
    else if(argc == 1)
        file_path = DEFAULT_FILE;
    else
    {
        printf("\n ERROR! Invalid arguments were passed during program startup.\n\n"
               " [HELP] You can pass the program the path to the file you need.\n"
               "\tFor example:"
               "\n\t./edit database.json\n\tor\n\t./edit /home/User/Documents/database.json\n\n");
        return 0;
    }

    list_of_people = calloc(1,sizeof(list_people_t));
    list_of_address = calloc(1,sizeof(list_address_t));

    /*if(read_file(file_path) != VALID) // Предназначено для открытия и считывания информации
        goto error;*/                  // из файла в специальном формате записи данных (см. listedit.c)
                                      // Чтобы использовать этот код, необходимо раскомментировать функцию
                                     // read_file, а также save_to_file, в файле listedit.c и их определение в listedit.h

    if(check_file_access(file_path) != VALID)
        goto error;

    if(read_json_file(file_path) != VALID)
        goto error;

    printf("\n*** Welcome to the people database editor! ***\n"
           " You are editing a file: %s\n"
           "\n Enter the 'show commands' command to view the command reference.\n", file_path);

    change_flag = 0;

    signal(SIGINT, signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    while((exit_signal_flag == 0) && (cmd_num != CMD_EXIT))
    {
        printf(">");
        cmd_num = process_command_name();
        execute_command(cmd_num, file_path);
    }

error:
    free(list_of_people);
    free(list_of_address);

    return 0;
}
