#include <stdio.h>
#include <stdlib.h> // для free(), calloc()
#include <signal.h> // для signal()

#include "commands.h"
#include "setdata.h"
#include "listedit.h"
#include "types.h"

#define DEFAULT_FILE "./database.txt"

/*Головная фунция, которой может передаваться путь до файла при запуске программы*/
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

    signal(SIGINT, SIG_IGN);

    while(cmd_num != CMD_EXIT)
    {
        printf("\n >");
        cmd_num = process_command_name();
        execute_command(cmd_num, file_path);
    }

no_file:
    free(list_of_people);
    free(list_of_address);

    return 0;
}
