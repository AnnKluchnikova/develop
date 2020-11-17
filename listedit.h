#pragma once

#include "types.h"

/*Проверка базы на непустоту*/
int not_empty_database(void);

/*Функция передачи указателя на уже существующий адрес*/
address_t *get_existing_address(char *street_name, int home_num);

/*Добавление записи в справочник адресов*/
address_t *add_new_address(char *street_name, int home_num);

/*Добавление записи в список людей*/
void add_new_person(char *name, unsigned int age, char *street_name, int home_num, int special_label);

/*Функция проверки полученных данных для последующего добавления*/
int add_correct_data_to_database(char *name, char *age, char *street_name, char *home_num, int show_message);

/*Функция считывания данных пользователя с консоли*/
int get_data_to_add(void);

/*Вывод списка имен на экран*/
void view_all_lists(void);

/*Вывод списка адресов на экран*/
void view_list_of_address(void);

/*Очистка списка адресов*/
int clear_list_of_address(void);

/*Очистка списка людей*/
int clear_list_of_people(void);

/*Полная очистка списка*/
int clear_all_lists(int show_message);

/*Поиск элемента по частичному совпадению с именем*/
int search_record_by_name_pattern(char *name);

/*Поиск элемента*/
void search_record_by_name(char *name);

/*Получения корректного имени для дальнейшего поиска в базе*/
int get_name_to_serch(void);

/*Вывод сообщений после работы функции удаления записи*/
int message_about_delete(int items_deleted);

/*Проверка на принадлежности адреса человеку*/
int address_belong_to_people(address_t *address);

/*Удаление адреса из списка*/
int delete_address_record(address_t *address);

/*Удаление элемента из списка по имени*/
int delete_person_record(char *name);

/*Получение корректного имени для последующего удаления записи*/
int get_name_to_delete(void);

/*Функция проверки доступности файла и проверки его содержимого*/
int read_file(char const *file_path);

/*Функция сохранения изменений в файл*/
int save_to_file(char const *file_path);