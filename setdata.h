#pragma once

/*Функция для считывания данных*/
int read_data(char *buf, int data_len);

/*Функция проверки строковых данных на корректность*/
int is_correct_string(char *buf);

/*Функция для преобразования возраста из строки в число*/
int get_age_as_number(char *buf);

/*Функция для преобразования номера дома из строки в число*/
int get_home_number_as_number(char *buf);
