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
    size_t argc;
    size_t argv_size;
};

typedef struct Command  Command;
typedef Command **      CommandVec;

FILE * get_input_stream(char * argv[]) {
    if (argv[1] == NULL) {
        // Interactive mode. Read command from stdin.
        return stdin;
    } else {
        // Batch mode. Read command from batch file.
        return fopen(argv[1], "r");
    }
}

char * read_line(FILE * infile) {
    char * line;
    if ( (line = calloc(4096, sizeof(char))) == NULL ) {	// TODO: variable length
        return NULL;
    }
    return fgets(line, sizeof(*line) * 4096, infile);
}

Command * parse_command(const char * command_string) {
    Command * command_struct;
    const char * tok_start;
    const char * tok_end;

    command_struct = calloc(1, sizeof(Command));
    command_struct->argc = 0;
    command_struct->argv_size = 1;
    command_struct->argv = calloc(command_struct->argv_size + 1, sizeof(*command_struct->argv));

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
        strncpy(command_struct->argv[command_struct->argc], tok_start, tok_end - tok_start);
        command_struct->argv[++command_struct->argc] = NULL;
        if (command_struct->argc == command_struct->argv_size) {      // Expand argv
            command_struct->argv_size *= 2;
            command_struct->argv = realloc(command_struct->argv,
                                           sizeof(*command_struct->argv) * (command_struct->argv_size + 1));
        }
    }
}

CommandVec parse_line(char * line) {
    CommandVec command_vec;
    size_t command_vec_size;
    size_t command_vec_cap;
    char * p;

    command_vec_size = 0;
    command_vec_cap = 1;
    command_vec = calloc(command_vec_cap + 1, sizeof(*command_vec));
    p = strtok(line, ";");

    while (p != NULL) {
        command_vec[command_vec_size++] = parse_command(p);
        command_vec[command_vec_size] = NULL;
        if (command_vec_size == command_vec_cap) {      // Expand command_vec
            command_vec_cap *= 2;
            command_vec = realloc(command_vec, sizeof(*command_vec) * (command_vec_cap + 1));
        }
        p = strtok(NULL, ";");
    }

    return command_vec;
}

// TODO:
int exec_commands(const CommandVec command_vec) {
    int i;
    for (i = 0; command_vec[i] != NULL; i++) {
        int j;
        printf("cmd: %s\n", command_vec[i]->cmd);
        printf("args: ");
        for (j = 0; j < command_vec[i]->argc; j++) {
            printf("%s ", command_vec[i]->argv[j]);
        }
        printf("\n");
    }
    printf("Done.\n");
    return 0;
}

int main(int argc, char * argv[]) {
    FILE * p_infile;
    char * line;

    if ((p_infile = get_input_stream(argv)) == NULL) {
        fprintf(stderr, "Failed to open input stream.\n");
        exit(1);
    }

    while ((line = read_line(p_infile)) != NULL && strcmp(line, "quit") != 0) {
        CommandVec cmd_vec;
        cmd_vec = parse_line(line);
        exec_commands(cmd_vec);
        free(line);
    }

    printf("quit\n");
    return 0;
}

