#ifndef GETJSON_H
#define GETJSON_H
/*Функция преобразует внутренний формат данных в формат json*/
int add_json_object_to_database(char const *file_path);

/*Функция записывает форматированный данные в файл json*/
int write_data_to_json_file(char const *file_path);
#endif