#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int history() {
	FILE *file;
	char path[PATH_MAX];
	snprintf(path, sizeof(path), "%s/.eonsh_history", getenv("HOME"));
	file = fopen(path, "a");
	fputs(buffer, file);
	fputc('\n', file);
	fclose(file);
}
