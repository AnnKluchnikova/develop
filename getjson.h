#ifndef GETJSON_H
#define GETJSON_H
/*Функция считывает данные файла json и записывает их во внутреннюю структуру*/
int read_json_file(char const *file_path);

/*Функция записывает форматированный данные в файл json*/
int write_data_to_json_file(char const *file_path);
#endif