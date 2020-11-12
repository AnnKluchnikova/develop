task: edit_database.c
	gcc -Wall -g edit_database.c f_data_entry.c f_database.c f_database_file.c -o edit
	#./edit