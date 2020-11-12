#pragma once
#include "types.h"

/*Проверка базы на непустоту*/
int not_empty_database();

/*Функция передачи указателя на уже существующий адрес*/
address_t *get_existing_address(char *street_name, int home_num);

/*Добавление записи в справочник адресов*/
address_t *add_new_address(char *street_name, int home_num);

/*Добавление записи в список людей*/
void add_new_person(char *name, unsigned int age, char *street_name, int home_num, int special_label);

/*Функция считывания данных пользователя с консоли*/
int get_data_to_add();

/*Вывод списка имен на экран*/
void view_all_lists();

/*Вывод списка адресов на экран*/
void view_list_of_address();

/*Очистка списка адресов*/
int clear_list_of_address();

/*Очистка списка людей*/
int clear_list_of_people();

/*Полная очистка списка*/
int clear_all_lists(int call);

/*Поиск элемента по частичному совпадению с именем*/
int search_record_by_name_pattern(char *name);

/*Поиск элемента*/
void search_record_by_name(char *name);

/*Получения корректного имени для дальнейшего поиска в базе*/
int get_name_to_serch();

/*Вывод сообщений после работы функции удаления записи*/
int message_about_delete(int items_deleted);

/*Проверка на принадлежности адреса человеку*/
int address_belong_to_people(address_t *address);

/*Удаление адреса из списка*/
int delete_address_record(address_t *address);

/*Удаление элемента по имени*/
int delete_person_record(char *name);

/*Получение корректного имени для последующего удаления записи*/
int get_name_to_delete();

