#pragma once

#define STRING_LEN 64
#define NUMBER_LEN 8

int change_flag; // Счетчик изменений для запроса сохранения в файл,
                       // если произошли какие-то изменения устанавливается флаг

enum CALL {        // Если вызов функции произошел с консоли,
    FILE_CALL,    // то по завршению ее выполнения  в консоле
    CONSOLE_CALL // будет выведено соответсвующее сообщение,
};              // а если из файла, то сообщений от функций не будет

enum EXIT_FUNC {
    ERROR = -1,
    VALID
};

typedef struct address_s
{
    char *street_name;
    int home_num;
    struct address_s *next;
    struct address_s *prev;
}address_t;

typedef struct list_address_s
{
    int item_counter;
    address_t *head;
    address_t *tail;
}list_address_t;

typedef struct people_s
{
    char *name;
    unsigned int age;
    address_t *address_point;
    struct people_s *next;
    struct people_s *prev;
} people_t;

typedef struct list_people_s
{
    int item_counter;
    people_t *head;
    people_t *tail;
}list_people_t;

list_people_t *list_of_people;
list_address_t *list_of_address;