Текущий проект состоит из 7 составных файлов и Make-файла для компиляции
Сборка проекта:

• edit_database.c - головная программа

• types.h - заг.ф. с глобальными переменными, структурами и макросами

• setdata.h/setdata.с - файлы, содержащие описанием функций работы со входными данными

• lstedit.h/lstedit.с - файлы, содержащие описанием функций работы со списками: добаление, удаление, поиск элемента и очистка, вывод на экран списков.

• filework.h/filework.с - файлы, содержащие описанием функций работы с файлами: чтение из файла, сохранение в файл

Задание:

• Организовать базу данных из двусвязных списоков

• Один список должен содержать неповторяющиеся адреса

• Второй список должен содержать данные человека, включая адрес

• Реализовать связь между списками через указатель

• Новый элемент должен добавляться в конец списка

• Вывод списка на экран должен произваодится с головы списка

• Создать функции удаления элементов из списка по имени человека (параметр name), при этом удаление записи человека не влечет за собой удаление записи соответствующего ему адреса

• Создать функцию удаления элемента из списка адресов, при этом необходимо проверить, что ссылки на данный адрес нет ни у одного человека

• Создать функцию поиска по частичному совпадению. Частичное совпадение предполагает совпадение по первым символам, соответсвтующие введенному шаблону

• Совместить частичный поиск с существующей функцие поиска по имени. При этом, если не находится полное совпадение по имени, то вывести список элементов по частичному поиску.

• Заменить мнемонические числа на словесные команды, добавить команду справки, которая будет выводить список имеющихся команд

• Заменить числовые константы в блоке switch case на макросы по выбору (define/enum)

• Реализовать возможность сохранять базу данных в файл и читать из файла

• Дать возможность пользователю задавать путь до файла при запуске программы, что предполагает наличие аргументов функции main

• Всегда производить попытки прочитать файл по дефолтному пути

• При чтении из файла всегда проверять корректность данных, если данные не корректны очистить списки и выйти из программы с ошибкой

• Добавить команду сохранения базы в файл, кроме этого при выходе из программы дать пользователю выбор на сохранение/не сохранение изменений

• Создать функцию для приема и обработки данных пользователя. Функция должна считывать данные с консоли и приводить к нужному виду, если это не получается сделать, то выдавать ошибку и при необходимости очищать ранее записанные данные
