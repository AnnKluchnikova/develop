#pragma once

#define STRING_LEN 64
#define NUMBER_LEN 8

extern char const *file_path;
volatile int change_flag;  // Счетчик изменений для запроса сохранения в файл,
                          // если произошли какие-то изменения устанавливается флаг

typedef enum MESSAGE {        // Если вызов функции произошел с консоли,
    NEED_MESSAGE,            // то по завршению ее выполнения  в консоле
    NO_MESSAGE              // будет выведено соответсвующее сообщение,
} message;                 // а если из файла, то сообщений от функций не будет

enum EXIT_FUNC {
    ERROR = -1,
    VALID
};

enum CMD{
    CMD_SHOW_CMD,
    CMD_GET_ALL_LISTS,
    CMD_CLEAR_ALL_LISTS,
    CMD_ADD_LIST_ITEM,
    CMD_FIND_RECORD_BY_NAME,
    CMD_EXIT,
    CMD_GET_ADDRESS_LIST,
    CMD_DELETE_PERSON,
    CMD_SAVE_DATABASE_IN_FILE
};

typedef struct address_s
{
    char *street_name;
    unsigned int home_num;
    struct address_s *next;
    struct address_s *prev;
}address_t;

typedef struct list_address_s
{
    unsigned int item_counter;
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
    unsigned int item_counter;
    people_t *head;
    people_t *tail;
}list_people_t;

list_people_t *list_of_people;
list_address_t *list_of_address;
