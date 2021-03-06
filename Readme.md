# **Версия программы 2.1.2**
* *Изменения*: При работе с форматом json убрана жествая привязка на размещение объектов данных. Т.е. описание данных человека таких, как имя, возраст, адрес (улица, дом) могут идти в каждом блоке в разном порядке.
* *Ограничения*: Ни один объект блока не может быть пустым или отсутствовать вовсе. Это ограничение возникает из-за строения внутреннего формата программы.

*Пример блока:*
```
"1":
    {
        "Name":"Ann Kluchnikova",
        "Age": "21",
        "Address":
        {
            "Street Name":"O.Koshevoy",
            "Home Number":"12"
        }
    }
    /*или*/
"1":
    {
        "Address":
        {
            "Home Number":"12",
            "Street Name":"O.Koshevoy"
        }
        "Age": "21",
        "Name":"Ann Kluchnikova"
    }
```
# **Сборка проекта v2.1.2**

Для осуществления сборки проекта необходимо скачать следующие файлы в одну директорию: main.c, listedit.c, listedit.h, types.h, setdata.c, setdata.h, commands.c, commands.h, getjson.c, getjson.h, Makefile.

Перед сборкой необходимо убедиться, что в системе установлена специальная библиотека json-c. Для ее установки используйте следующую команду:
`sudo apt-get install libjson-c-dev`

Запуск сборки производится по команде `make`, находясь в директории, куда ранее были скачены указанные файлы.

Кроме этого, перед началом запуска программы необходимо скачать файл database.json, который является файлом по умолчанию. При запуске программы с помощью команды `./edit`, она будет использовать именно этот файл. Чтобы воспользоваться программой с другим файлом, необходимо на этапе запуска передать ей путь до файла с форматом json, например, `./edit /home/BD.json`.

## **Состав файлов:**

* *main.c* - основной файл с головной функцией main, к которому подключаются все имеющиеся заголовочные файлы
* *listedit.c / listedit.h* - файлы с описанием функций работы со списками
* *types.h* - файл с описанием использующихся типов, макросами и внешними переменными
* *setdata.c / setdata.h* - файлы с описанием функция работы с данными
* *commands.c / commands.h* - файлы с описанием функций обработки и исполнения команд пользователя
* *getjson.c / getjson.h* - файлы с описанием функция работы с данными и файлами в формате json.

# **Сопутствующий проект**
В исходном коде проекта находятся закомментированные части с небольшим описанием. Данный код остался с рабочей версии v1.1.2 ([https://github.com/AnnKluchnikova/develop/tree/assembly_project]) и дает возможность использовать программу для редактирования текстовых файлов с форматом .txt. Однако, чтобы пользоваться файлами расширения .txt или без какого либо расширения(просто текстовый файл доступный для чтения и записи), то необходимо заранее раскомментировать части кода, в частности, функции `read_file` и `save_to_file` в файле listedit.c.

**ВАЖНО!** Чтобы программа при этом работала корректно, будет необходимо найти использование этих функций в других частях проекта и расскомментровать или добавить нужные куски кода.

**ОГРАНИЧЕНИЯ!** Программа предполагает редактирвоани одного из форматов файлов: .json или .txt, поэтому при добавлении функций/библиотек для одного формата исключает использование функций/библиотек другого.  

# **Задание:**

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

