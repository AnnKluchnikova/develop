#include <stdio.h>
#include <stdlib.h> // для free(), calloc()
#include <signal.h> // для signal(), для raise()

#include "commands.h"
#include "listedit.h"
#include "types.h"

#define DEFAULT_FILE "./database.txt"

unsigned char exit_flag = 0; // Флаг выхода из программы по сигналу Ctrl+C
char const *file_path;      // Путь до файла, с данными которого будет работать программа

/*Функция обработки сигналов*/
void signal_handler(int signo)
{
    if(signo == SIGINT)
    {
        execute_command(CMD_EXIT, file_path);
        exit_flag = 1;
    }
    else if(signo == SIGUSR1)
        printf("Received SIGUSR1!\n");
    else if(signo == SIGUSR2)
        printf("Received SIGUSR2!\n");
}

/*Головная фунция, которой может передаваться путь до файла при запуске программы*/
int main(int argc, char const *argv[])
{
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
           "\n Enter the 'show commands' command to view the command reference.\n", file_path);

    change_flag = 0;
    int cmd_num = 0;

    signal(SIGINT, signal_handler);
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    while((exit_flag == 0) && (cmd_num != CMD_EXIT))
    {
        printf(">");
        cmd_num = process_command_name();
        execute_command(cmd_num, file_path);
    }

no_file:
    free(list_of_people);
    free(list_of_address);

    return 0;
}
