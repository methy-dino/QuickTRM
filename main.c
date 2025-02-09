#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pty.h>
#include <signal.h>
int main(int argC, char** args){
	char* user = getlogin();
	int userL = 0;
	while (user[userL] != '\0') {
		userL++;
	}
	// "cd " + "/home/" + username + "/.quickTRM"+ '\0'
	char path[3+6+userL+11];
	path[3] = '/';
	path[4] = 'h';
	path[5] = 'o';
	path[6] = 'm';
	path[7] = 'e';
	path[8] = '/';
	for (int i = 0; i < userL; i++){
		path[i+6+3] = user[i];
		printf("%c", user[i]);
	}
	path[userL+9] = '/';
	path[userL+10] = '.';
	path[userL+11] = 'q';
	path[userL+12] = 'u';
	path[userL+13] = 'i';
	path[userL+14] = 'c';
	path[userL+15] = 'k';
	path[userL+16] = 'T';
	path[userL+17] = 'R';
	path[userL+18] = 'M';
	path[userL+19] = '\0';
	return 0;
}
