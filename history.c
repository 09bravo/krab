#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

void history(const char *buffer) {
	FILE *file;
	char path[PATH_MAX];
	snprintf(path, sizeof(path), "%s/.krab_history", getenv("HOME"));
	file = fopen(path, "a");
	fputs(buffer, file);
	fputc('\n', file);
	fclose(file);
}
