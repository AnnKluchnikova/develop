#pragma once

/*Функция для считывания данных*/
int read_data(char *buf, int data_len);

/*Функция для проверки непустоты строки*/
int is_not_null_string(char *buf);

/*Функция проверки строковых данных на корректность*/
int is_correct_string(char *buf);

/*Функция для распознования возраста "0 лет"*/
int zero_is_age(char *buf);

/*Функция для преобразования возраста из строки в число*/
int get_age_as_number(char *buf);

/*Функция для преобразования номера дома из строки в число*/
int get_home_num_as_number(char *buf);

/*Функция проверки полученных данных для последующего добавления*/
int add_correct_data_to_database(char *name, char *age, char *street_name, char *home_num, int call);