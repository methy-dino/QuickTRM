#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pty.h>
#include <signal.h>
#include "libs/string.h"
#include <fcntl.h>
String* configs;
String* editor;
String* terminal;
String* home;
int local = 0;
void defaults(){
	struct stat st = {0};
	if (stat(home->string, &st) != 0){
		printf("no file storage folder found, creating a new one\n");
		mkdir(home->string, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	configs = cloneStr(home);
	appendPtr(configs, "/settings.txt", 9);
	int settings;
	if (stat(configs->string, &st) != 0) {
		settings = open(configs->string, O_WRONLY | O_APPEND | O_CREAT, 0644);
		printf("settings not found, initializing new settings\n");
		write(settings, "gnome-terminal -- \nvim ", 22);
		close(settings);
	}
}
void loadSettings(){
	appendPtr(home, "\"", 1);
	FILE* file;
    file = fopen(configs->string, "r");
	terminal = emptyStr(64);
	char read[128];
	fgets(read, 128, file);
	appendNoLen(terminal, read);
	terminal->length--;
	terminal->string[terminal->length] = '\0';
	editor = emptyStr(64);
	fgets(read, 128, file);
	appendNoLen(editor, read);
	fclose(file);
	home->length--;
	home->string[home->length] = '\0';
}
void listSaves(){
	DIR* folder = opendir(home->string);
	int currLen = 0;
	struct dirent* currFile;
	printf("the current saved terminals are: \n");
		while ((currFile=readdir(folder)) != NULL){
			if (!strcmp (currFile->d_name, "."))
           			 continue;
        		if (!strcmp (currFile->d_name, ".."))
            			continue;
			// cuts .sh out of the name and doesn't log undesired files
			currLen = 0;
			while (currFile->d_name[currLen] != '\0'){
				currLen++;
			}
			if (currFile->d_name[currLen - 1] == 'h' && currFile->d_name[currLen - 2] == 's'){
				currFile->d_name[currLen - 3] = '\0';
				printf("· %s\n", currFile->d_name);
			}
		}
}
void delete(char** args, int argL){
	// We can clone home, but since we won't use it further, it is useless.
	//String* currFilePath = cloneStr(home);
	appendPtr(home, "/", 1);
	// save length for terminator shenanigans.
	int prevL = home->length;
	if (strcmp(args[2], "ALL SAVES") == 0){
		DIR* folder = opendir(home->string);
		int currLen = 0;
		struct dirent* currFile;
		while ((currFile=readdir(folder)) != NULL){
			if (!strcmp (currFile->d_name, ".")){
           			 continue;
			}
      if (!strcmp (currFile->d_name, "..")){
            			continue;
			}
			currLen = 0;
			while (currFile->d_name[currLen] != '\0'){
				currLen++;
			}
			if (currFile->d_name[currLen - 1] == 'h' && currFile->d_name[currLen - 2] == 's'){
				appendPtr(home, currFile->d_name, currLen);
				currFile->d_name[currLen-3] = '\0';
				if (remove(home->string) == 0){
						printf("removed saved terminal named \"%s\"\n", currFile->d_name);
					} else {
						printf("\033[31m failed to remove saved terminal named \"%s\"\n\033[0m", currFile->d_name);
					}	
			}
			home->length = prevL;
			home->string[prevL] = '\0';
		}
		return;
	}
	for (int i = 2; i < argL; i++){
		appendNoLen(home, args[i]);
		appendPtr(home, ".sh", 3);
		if (remove(home->string) == 0){
			printf("removed saved terminal named \"%s\"\n", args[i]);
		} else {
			printf("\033[31m failed to remove saved terminal named \"%s\"\n\033[0m", args[i]);
		}
		home->length = prevL;
		home->string[prevL] = '\0';
	}
}
void loadFiles(char** args, int argL, int start){
	struct stat st = {0};
	appendPtr(home, "/", 1);
		String* command;
		int prevHL = home->length;
		int i;
		if (local){
			command = emptyStr(20);
		} else {
			command = cloneStr(terminal);
		}
		i = start;
		appendPtr(command, "\"", 1);
		int prevCL = command->length;
		if (strcmp(args[i],"ALL SAVES") == 0){
			DIR* folder = opendir(home->string);
			int currLen = 0;
			struct dirent* currFile;
			while ((currFile=readdir(folder)) != NULL){
				if (!strcmp (currFile->d_name, ".")){
        	   		continue;
				}
      	if (!strcmp (currFile->d_name, "..")){
        	    	continue;
			}
			currLen = 0;
			while (currFile->d_name[currLen] != '\0'){
				currLen++;
			}
			if (currFile->d_name[currLen - 1] == 'h' && currFile->d_name[currLen - 2] == 's'){
				appendStr(command, home);
				appendPtr(command, currFile->d_name, currLen);
				appendPtr(command, "\"", 1);
				system(command->string);
			}
			command->length = prevCL;
			command->string[prevCL] = '\0';
		}
		return;
		}

		while (i < argL){
			appendNoLen(home, args[i]);
			appendPtr(home, ".sh", 3);
			if (stat(home->string, &st) != 0){
				printf("\033[31m invalid terminal to load: \"%s\"\nPROCESS ABORTED \n\033[0m", args[i]);
			} else {
					appendStr(command, home);
					appendPtr(command, "\"", 1);
					system(command->string);
			}
			i++;
			command->length = prevCL;
			command->string[prevCL] = '\0';
			home->length = prevHL;
			home->string[prevHL] = '\0';
			}
}
void importFiles(char** args, int argL){
	String* file_cp = emptyStr(64);
	appendPtr(file_cp, "cp ", 3);
	int prev_len = file_cp->length;
	int file_l = 0;
	int has_term = 0;
	int j = 0;
	for (int i = 2; i < argL; i++){
		has_term = 0;
		while (args[i][file_l] != '\0') {
			if (args[i][file_l] == '.'){
				has_term = 1;
			}
			else if (args[i][file_l] == '/') {
				j = file_l;
				if (args[i][file_l+1] == '.'){ 
					file_l++;
				}
				has_term = 0;
			}
			file_l++;
		}
			appendPtr(file_cp, args[i], file_l);
			//get filename len
			if (has_term == 1){
					if (args[i][file_l - 1] == 'h' && args[i][file_l - 2] == 's' && args[i][file_l - 3] == '.'){
					appendPtr(file_cp, " ", 1);
					appendStr(file_cp, home);
					appendSubPtr(file_cp, args[i], j, file_l);
					system(file_cp->string);
				} else {
					printf("\033[31m file \"%s\" is not a shell file! \n\033[0u", args[i]);
				}
			} else {
				appendPtr(file_cp, "/", 1);
				int dir_l = file_cp->length;
				// DONE.
				String* full_name = emptyStr(64);
				if (args[i][0] == '.'){
					char cwd[256];
					getcwd(cwd, 256);
					appendNoLen(full_name, cwd);
					appendSubPtr(full_name,args[i], 1, file_l);
				} else {
					appendPtr(full_name, args[i], file_l);
				}
				DIR* folder = opendir(full_name->string);
				if (folder == NULL) {
					printf("ERR: FOLDER \"%s\" NOT FOUND \n", full_name->string);
					continue;
				}
				struct dirent* curr_file;
				while ((curr_file = readdir(folder)) != NULL){
					int sub_l = strlen(curr_file->d_name);
					if (sub_l > 2 && curr_file->d_name[sub_l-1] == 'h' && curr_file->d_name[sub_l-2] == 's' && curr_file->d_name[sub_l-3] == '.'){
						appendPtr(file_cp, curr_file->d_name, sub_l);
						appendPtr(file_cp, " ", 1);
						appendStr(file_cp, home);
						system(file_cp->string);
					}
					file_cp->length = dir_l;
				}
			}
			//printf("%s\n",file_cp->string);
		file_cp->length = prev_len;
		file_cp->string[prev_len] = '\0';
	}
}
void exportFiles(char** args, int argL){
	if (argL < 3){
		printf("no output folder specified.");
		return;
	}
	// TODO: check out validity.
	String* out_cp = emptyStr(64);
	String* out_dir = emptyStr(64);
	appendPtr(out_cp, "cp ", 3);
	appendStr(out_cp, home);
	appendPtr(out_cp, "/", 1);
	if (args[2][0] == '.'){
		char cwd[256];
		getcwd(cwd, 256);
		appendNoLen(out_dir, cwd);
		appendSubPtr(out_dir, args[2], 1, strlen(args[2]));
	} else {
		appendNoLen(out_dir, args[2]);
	}
	if (out_dir->string[out_dir->length] != '/'){
		appendPtr(out_dir, "/", 1);
	}
	DIR* home_fol = opendir(home->string);
	struct dirent* curr_f;
	int prev_l = out_cp->length;
	while ((curr_f=readdir(home_fol))!= NULL){
		if (curr_f->d_name[0] == '.' && (!(curr_f->d_name[1]) || curr_f->d_name[1] == '.')){
				continue;
			}
		appendNoLen(out_cp, curr_f->d_name);
		appendPtr(out_cp, " ", 1);
		appendStr(out_cp, out_dir);
		printf("%s\n", out_cp->string);
		system(out_cp->string);
		out_cp->length = prev_l;

	}
	closedir(home_fol);
}
int main(int argL, char** args){
	char* user = getlogin();
	int userL = 0;
	while (user[userL] != '\0') {
		userL++;
	}
	//"/home/" + username + "/.quickTRM"+ '\0'
	char path[6+userL+10];
	path[0] = '/';
	path[1] = 'h';
	path[2] = 'o';
	path[3] = 'm';
	path[4] = 'e';
	path[5] = '/';
	for (int i = 0; i < userL; i++){
		path[i+6] = user[i];
	}
	path[userL+6] = '/';
	path[userL+7] = '.';
	path[userL+8] = 'q';
	path[userL+9] = 'u';
	path[userL+10] = 'i';
	path[userL+11] = 'c';
	path[userL+12] = 'k';
	path[userL+13] = 'T';
	path[userL+14] = 'R';
	path[userL+15] = 'M';
	path[userL+16] = '\0';
	home = buildStr(path, userL+16);
	defaults();
	loadSettings();
	if (argL == 2 && strcmp(args[1], "clone") == 0){
		// getcwd doesn't finish the string????
		// data will PROBABLY BE SAFE, but just to be sure
		String* cloner = buildStr("#!/bin/bash \n", 13);
		appendStr(cloner, terminal);
		// remove the space after "--"
		cloner->length--;
		appendNoLen(cloner, "working-directory=\"");
		char cwd[256];
		getcwd(path, 256);
		growStr(cloner, 257);
		int i = 0;
		while (path[i] != 0){
			cloner->string[cloner->length] = path[i];
			cloner->length++;
			i++;
		}
		cloner->string[cloner->length] = '\"';
		cloner->length++;
		cloner->string[cloner->length] = '\0';
		printf("cloning directory using:\"%s\" \n", cloner->string);
		system(cloner->string);
		return 0;
	} else if (argL > 1 && strcmp(args[1], "load") == 0){
		int start = 2;
		if (strcmp(args[2], "local") == 0){
			local = 1;
			start++;
		}
		loadFiles(args, argL, start);
	} else if (argL > 1 && strcmp(args[1], "create") == 0){
	if (argL != 3){
		printf("\033[31m failed to parse arguments, invalid quantity \n\033[0m");
		return 0;
	}
	appendPtr(home, "/", 1);
	appendNoLen(home, args[2]);
	appendPtr(home, ".sh", 3);
	String* editorPath = emptyStr(home->length + 2);
	appendPtr(editorPath, "\"", 1);
	appendStr(editorPath, home);
	appendPtr(editorPath, "\"", 1);
	appendStr(editor, editorPath);
	printf("%s \n", home->string);
	if (access(home->string, F_OK) != 0){
		printf("creating terminal template %s: \n", args[2]);
		String* flagExec = buildStr("chmod +x ", 9);
		appendStr(flagExec, editorPath);
		String* baseCode = buildStr("#/bash/sh\n",10);
		appendPtr(baseCode, "cd ", 3);
		char currDir[256];
		getcwd(currDir, 256);
		appendNoLen(baseCode, currDir);
		appendPtr(baseCode, "\n", 1);
		appendPtr(baseCode, "exec bash", 9);
		int file = open(home->string, O_WRONLY | O_APPEND | O_CREAT, 0644);
		write(file, baseCode->string, baseCode->length);
		system(flagExec->string);
		system(editor->string);
	} else {
		printf("a saved terminal with that name already exists, do you wish to edit it? (y/n)  ");
		char input;
		while (0==0){
			scanf("%c", &input);
			if ((input == 'y' || input == 'Y')){
				system(editor->string);
				return 0;
			} else if ((input == 'n' || input == 'N')){
				return 0;
			}
		}
	}
	} else if (argL > 1 && (strcmp(args[1], "delete") == 0 || strcmp(args[1], "remove") == 0)){
		delete(args, argL);
	} else if (argL > 1 && (strcmp(args[1], "list") == 0 || strcmp(args[1], "view") == 0 || strcmp(args[1], "saves") == 0)){
		listSaves();
	} else if (argL > 2 && strcmp(args[1], "editor") == 0){
		FILE* set = fopen(configs->string, "w");
		fwrite(terminal->string, terminal->length, 1, set);
		fwrite("\n", 1, 1, set);
		int i = 0;
		while (args[2][i] !=  '\0'){
			i++;
		}
		fwrite(args[2], i, 1, set);	
		printf("· terminal:\"%s\" \n", terminal->string);
		printf("· editor: \"%s\" \n", args[2]);
		fclose(set);
	} else if (argL > 2 && strcmp(args[1], "terminal") == 0){
		FILE* set = fopen(configs->string, "w");
		int i = 0;
		while (args[2][i] !=  '\0'){
			i++;
		}
		fwrite(args[2], i, 1, set);
		fwrite("\n", 1, 1, set);
		fwrite(editor->string, editor->length, 1, set);
		fclose(set);
		printf("· terminal:\"%s\" \n", args[2]);
		printf("· editor: \"%s\" \n", editor->string);
	} else if (argL > 1 && strcmp(args[1], "settings") == 0){	
		printf("· terminal:\"%s\" \n", terminal->string);
		printf("· editor: \"%s\" \n", editor->string);
	} else if (argL > 1 && strcmp(args[1], "help") == 0){
		printf("create <name>\nrunning quickTRM create <name> will attempt to create a shell save in it's internal folder (~/.quickTRM), if a save already exists with that name, it will ask confirmation to edit it.\n\n");
		printf("delete <names>\ndeletes all saves listed, which are separated by spaces, if it fails to delete any, it will throw an error and continue.\n\n");
		printf("load <names>\nloads the specified terminals within names, separated by spaces, throws an error for each that is non-existant.\n\n");
		printf("load local <names>\nloads the specified terminals within names, exactly as load does, except it does not open new terminal windows.\n\n");
		printf("list\nlists all the saved files on the .quickTRM directory, also runs if called quickTRM view.\n\n");
		printf("clone\nclones the current terminal by opening another terminal in the same directory.\n\n");
		printf("editor <name>\nchanges the editor on the configurations file (default is \"vim \"), don't forget to quote the name and put a space in the end.\n\n");
		printf("terminal <name>\nchanges the terminal on the configurations file (default is gnome-terminal -- ).\n\n");
		printf("settings\ndisplay the currently set terminal, and editor, between quotes.\n\n");	
		printf("export <fPath>\n copies all the saved data of quickTRM to the specified directory\n");
		printf("import <fPath>\n imports the shell files from <fPath> to quickTRM's internal folder\n");
		printf("help\ndisplays information about all the quickTRM arguments and commands\n");
	} else if (argL > 1 && strcmp(args[1], "import") == 0){
		importFiles(args, argL);
	} else if (argL > 1 && strcmp(args[1], "export") == 0){
		exportFiles(args, argL);
	} else {
		String* fPath = cloneStr(home);
		// implicit file opening
		int start = 1;
		if (argL > 1 && strcmp(args[start], "local") == 0){
			local = 1;
			start++;
		}
		appendNoLen(fPath, args[start]);
		struct stat st = {0};
		if (stat(fPath->string, &st) == 0){
			loadFiles(args, argL, start);
		} else {
			printf("\033[31m unrecognized command\n\033[0m");
		}
		// no need to free "fPath", since it is right before the process end.
	}
	return 0;
}
