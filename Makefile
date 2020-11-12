task: edit_database.c
	gcc -Wall -g edit_database.c setdata.c lstedit.c filework.c -o edit
	#./edit