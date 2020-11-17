task: main.c
	gcc -Wall -g main.c setdata.c listedit.c commands.c -o edit
	#./edit