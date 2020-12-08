#pragma once

#include "types.h"

/*Функция проверки полученных данных для последующего добавления*/
int add_correct_data_to_database(char *name, char *age, char *street_name, char *home_num, message show_message);

/*Функция считывания данных пользователя с консоли*/
int get_data_to_add(void);

/*Вывод списка на экран*/
void view_all_lists(void);

/*Вывод списка адресов на экран*/
void view_list_of_address(void);

/*Полная очистка списка*/
int clear_all_lists(message show_message);

/*Получения корректного имени для дальнейшего поиска в базе*/
int get_name_to_serch(void);

/*Получение корректного имени для последующего удаления записи*/
int get_name_to_delete(void);

/*Функция проверки доступности файла и проверки его содержимого*/
//int read_file(char const *file_path);

/*Функция сохранения изменений в файл*/
//int save_to_file(char const *file_path);