#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "lstedit.h"
#include "setdata.h"
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
