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
	if (stat(str->string, &st) != 0){
		mkdir(str->string, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	if (argL == 2 && args[1] == "clone"){

	}
	if (argL > 1 && strcmp(args[2], "load")){
		if (argL < 3){
			printf("unspecified terminal to load \n ABORTED \n");
			return 1;
		} else if (argL > 3){
			printf("invalid parameter quantity \n ABORTED \n");
			return 1;
		}
		appendNoLen(str, args[2]);
		if (stat(str->string, &st) != 0){
			printf("invalid terminal to load \n ABORTED \n");
		}	
	}
	printf("done\n");
	return 0;
}
