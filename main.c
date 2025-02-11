#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
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
	}
	if (argL > 1 && strcmp(args[1], "load") == 0){
		appendPtr(str, "/", 1);
		appendNoLen(str, args[2]);
		appendPtr(str, ".sh", 3);
		String* command = buildStr("gnome-terminal -- ", 18);
		appendStr(command, str);
		printf("%s", str->string);
		if (argL < 3){
			printf("unspecified terminal to load \nPROCESS ABORTED \n");
			fflush(stdout);
			return 0;
		} else if (argL > 3){
			printf("invalid parameter quantity \nPROCESS ABORTED \n");
			fflush(stdout);
			return 0;
		}
		//appendNoLen(command, args[2]);
		printf("==%d" ,stat("/home/meth/.quickTRM/teste.sh", &st));
		if (stat(str->string, &st) != 0){
			printf("invalid terminal to load \nPROCESS ABORTED \n");
			} else {
				system(command->string);
			}	
		}
	if (strcmp(args[1], "create") == 0){
		if (argL != 3){
			printf("failed to parse arguments, invalid quantity \n");
			return 0;
		}
		printf("creating terminal template %s: \n", args[2]);
		appendPtr(str, "/", 1);
		appendNoLen(str, args[2]);
		appendPtr(str, ".sh", 3);
		//make file and stuffies
		int file = open(str->string, O_WRONLY | O_APPEND | O_CREAT, 0644);
		write(file, "ahdahds", 7);  
	}
	return 0;
}
