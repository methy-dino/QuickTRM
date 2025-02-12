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
#include <security/pam_appl.h>
#include <fcntl.h>

#include <security/pam_modules.h>


#include <security/pam_ext.h>
#define CLONESCRIPT "#!/bin/bash \ngnome-terminal --working-directory=\" "
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
	String* str = buildStr(path, userL+16);
	struct stat st = {0};
	//pam_start();
	if (stat(str->string, &st) != 0){
		mkdir(str->string, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	if (argL == 2 && strcmp(args[1], "clone") == 0){
		// getcwd doesn't finish the string????
		// data will PROBABLY BE SAFE, but just to be sure
		char* path = (char*) calloc(128, 128);
		String* cloner = buildStr("#!/bin/bash \ngnome-terminal --working-directory=\"",49);
		getcwd(path, 128);
		growStr(cloner, 128);
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
		appendPtr(str, "/", 1);
		appendNoLen(str, args[2]);
		appendPtr(str, ".sh", 3);
		String* command = buildStr("gnome-terminal -- ", 18);
		appendStr(command, str);
		printf("%s", str->string);
		if (argL < 3){
			printf("\033[31m unspecified terminal to load \nPROCESS ABORTED \n\033[0m");
			fflush(stdout);
			return 0;
		} else if (argL > 3){
			printf("\033[31m invalid parameter quantity \nPROCESS ABORTED \n\033[0m");
			fflush(stdout);
			return 0;
		}
		//appendNoLen(command, args[2]);
		if (stat(str->string, &st) != 0){
			printf("\033[31m invalid terminal to load \nPROCESS ABORTED \n\033[0m");
			} else {
				system(command->string);
			}	
		} else if (strcmp(args[1], "create") == 0){
		if (argL != 3){
			printf("\033[31m failed to parse arguments, invalid quantity \n\033[0m");
			return 0;
		}
		String* openVim = buildStr("vim ", 4);
		appendPtr(str, "/", 1);
		appendNoLen(str, args[2]);
		appendPtr(str, ".sh", 3);
		appendStr(openVim, str);
		if (access(str->string, F_OK) !=0){
			printf("creating terminal template %s: \n", args[2]);
			String* flagExec = buildStr("chmod +x ", 9);
			printf("a\n");
			appendStr(flagExec, str);
			String* baseCode = buildStr("#/bash/sh\n",10);
			appendPtr(baseCode, "cd ", 3);
			char currDir[256];
			getcwd(currDir, 256);
			appendNoLen(baseCode, currDir);
			//appendNoLen(baseCode, currDir);
		//write(file, "#/bash/sh\n", 10);
		//write(file, "cd ", 3);
	        //write(file, str->string, str->length);
		//write(file, "\n", 1);
		//write(file, "exec bash", 9);	
			//printf("%d <-> %d\n", baseCode->length, baseCode->maxCapacity);
			appendPtr(baseCode, "\n", 1);
			appendPtr(baseCode, "exec bash", 9);
			int file = open(str->string, O_WRONLY | O_APPEND | O_CREAT, 0644);
			//write(file, "aa", 2);
			write(file, baseCode->string, baseCode->length);
			//system(flagExec->string);
			system(openVim->string);
		} else {
			printf("a saved terminal with that name already exists, do you wish to edit it? (y/n) ");
			char input;
			while (0==0){
				scanf("%c", &input);
				if (input == 'y' || input == 'Y'){
					system(openVim->string);
					return 0;
				} else if (input == 'n' || input == 'N'){
					return 0;
				}
			}
		}
	} else if (strcmp(args[1], "delete") == 0){
		appendPtr(str, "/", 1);
		for (int i = 2; i < argL; i++){
			String* currFilePath = cloneStr(str);
			appendNoLen(currFilePath, args[i]);
			appendPtr(currFilePath, ".sh", 3);
			if (remove(currFilePath->string) == 0){
				printf("removed saved terminal named %s\n", args[i]);
			} else{
				printf("\033[31m failed to remove saved terminal named %s\n\033[0m", args[i]);
			}
		}
	} else if (strcmp(args[1], "list") == 0 || strcmp(args[1], "view") == 0 || strcmp(args[1], "saves") == 0){
		DIR* folder = opendir(str->string);
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
	} else {
		printf("\033[31m unrecognized command\n\033[0m");
	}
	return 0;
}
