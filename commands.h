#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int cd(char* path) {
	if(path == NULL) {
		path = getenv("HOME");
	}
	if(chdir(path) != 0) {
		printf("dish: \"%s\" no such a directory", path);
		return 1;
	}
	return 0;
}
int help() {
	puts("olight0's dish, by the community for community.");
	puts("the help command is currently still on the work.");
	return 1;
}
#endif
