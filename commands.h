#pragma once

#include "types.h"

/*Запрос о сохранении базы перед выходом из программы*/
void ask_question_about_save(char const *file_path);

/*Функция обработки команды, которая запрашивает имя команды 
и возвращает ее номер-макрос для выполнение*/
int process_command_name(void);

/*Функция выполнения команды, которая получает номер-макрос команды
и вызывает соответствующие функции*/
void execute_command(int cmd_num, char const *file_path);
