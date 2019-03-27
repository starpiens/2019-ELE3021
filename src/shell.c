#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * A struct representing a single command.
 */
struct Command {
    char * cmd;
	char ** argv;
	unsigned int argc;
    unsigned int argv_size;
};

typedef struct Command  Command;
typedef Command **      CommandVec;

char * read_line(FILE * infile) {
	char * line;
	if ( (line = calloc(4096, sizeof(char))) == NULL ) {	// TODO: variable length
		return NULL;
	}
	return fgets(line, sizeof(line), infile);
}

Command * parse_command(const char * command_string) {
	Command * command_struct;
	command_struct = calloc(1, sizeof(Command));

	const char * tok_start;
	const char * tok_end;

	// Copy cmd.
	tok_start = command_string;
	while (isspace(*tok_start) && *tok_start != '\0') {
		tok_start++;
	}
	if (*tok_start == '\0') {	// command_string is a whitespace string or empty string.
		return NULL;
	}
	tok_end = tok_start;
	while (!isspace(*tok_end) && *tok_end != '\0') {
		tok_end++;
	}
	command_struct->cmd = calloc(tok_end - tok_start + 1, sizeof(char));
	strncpy(command_struct->cmd, tok_start, tok_end - tok_start);

	// Copy args.
	command_struct->argc = 0;
	command_struct->argv_size = 16;
	command_struct->argv = calloc(command_struct->argv_size, sizeof(char *));
	while (1) {
		tok_start = tok_end;
		while (isspace(*tok_start) && *tok_start != '\0') {
			tok_start++;
		}
		if (*tok_start == '\0') {
			return command_struct;
		}
		tok_end = tok_start;
		while (!isspace(*tok_end) && *tok_end != '\0') {
			tok_end++;
		}
		command_struct->argv[command_struct->argc] = calloc(tok_end - tok_start + 1, sizeof(char));
		if (++command_struct->argc == command_struct->argv_size) {
			command_struct->argv_size *= 2;
			command_struct->argv = realloc(command_struct->argv, command_struct->argv_size);
		}
	}
}

// TODO:
CommandVec parse_line(char * line) {
	char * p_start = line;
	do {
		p_start = strtok(p_start, ";");
	} while ( (p_start = strtok(p_start, ";")) );
}

// TODO:
int exec_commands(const CommandVec command) {
	return 0;
}

int main(int argc, char * argv[]) {

	// TODO: move this to seperated function
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
	    CommandVec cmd_vec;
	    cmd_vec = parse_line(line);
	    exec_commands(cmd_vec);
	    free(line);
	}

	printf("quit\n");
	return 0;
}

