#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main(int argc, char * argv[]) {

	// TODO: move this to seperate function
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
	while ((line = read_line(p_infile)) != NULL
		&& strcmp(line, "quit") != 0) {
	    CommandNode * p_cmd;
	    p_cmd = parse_line(line);
	    while (p_cmd != NULL) {
	    	exec_command(p_cmd);
	    	p_cmd = p_cmd->p_nxt;
	    }
	    free(line);
	}

	printf("quit\n");
	return 0;
}

char * read_line(FILE * infile) {
    char * line;
    if ( (line = malloc(4096)) == NULL ) {	// TODO: variable length
    	return NULL;
    }
	return fgets(line, sizeof(line), infile);
}

CommandNode parse_command(const char * cmd_start, const char * cmd_end) {
	CommandNode command;
	memset(&command, 0, sizeof(command));

	const char * tok_start;
	int tok_len;

	// Copy cmd
	tok_start = cmd_start;
	while (*tok_start == ' ') {
		tok_start++;
		if (tok_start == cmd_start) {	// No binary name
			return command;
		}
	}
	tok_len	= 0;
	while (*(tok_start + tok_len) != ' ' && tok_start + tok_len < cmd_end) {
		tok_len++;
	}
	command.cmd = malloc(sizeof(char) * (tok_len + 1));
	strncpy(command.cmd, tok_start, tok_len);

	// Copy args
	// TODO: variable length argv
	command.argv = malloc(sizeof(char *) * 10);
	int idx_argv = 0;
	while (tok_start < cmd_end) {
		while (*tok_start == ' ' && tok_start < cmd_start) {
			tok_start++;
		}
		tok_len = 0;
		while (*(tok_start + tok_len) != ' ' && tok_start + tok_len < cmd_end) {
			tok_len++;
		}
		command.argv[idx_argv] = malloc(sizeof(char) * (tok_len + 1));
		strncpy(command.argv[idx_argv], tok_start, tok_len);
		idx_argv++;
	}

	return command;
}

// TODO:
CommandList parse_line(char * line) {
	char * p_start = line;
    do {
        p_start = strtok(p_start, ";");
    } while ( (p_start = strtok(p_start, ";")) );
}

// TODO:
int exec_command(const CommandNode * command) {
	return 0;
}