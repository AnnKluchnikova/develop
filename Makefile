task: main.c
	gcc -Wall -g main.c setdata.c listedit.c commands.c getjson.c -o edit $(shell pkg-config --libs --cflags json-c)
	#./edit