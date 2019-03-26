#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "shell.h"

int main(int argc, char * argv[]) {

	FILE * p_infile;

	if (argc == 1) {
	    // Interactive mode. Read command from stdin.
		p_infile = stdin;
	} else {
	    // Batch mode. Read command from batch file.
		p_infile = fopen(argv[1], "r");
		if (p_infile == NULL) {
			fprintf(stderr, "Cannot open batch file: %s\n", argv[1]);
			exit(1);
		}
	}

	char * line;
	while ((line = read_line(p_infile)) != NULL) {
	    fprintf(stdout, "%s", line);
	}

	return 0;
}

char * read_line(FILE * infile) {
    char * line;
    line = (char *)malloc(4096);
	if (line == NULL) return NULL;
	return fgets(line, sizeof(line), infile);
}