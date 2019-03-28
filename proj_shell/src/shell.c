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
    pid_t pid;
};

typedef struct Command  Command;
typedef Command **      CommandVec;

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
        free(command_struct->argv);
        free(command_struct);
        return NULL;
    }
    tok_end = tok_start;
    while (!isspace(*tok_end) && *tok_end != '\0') {
        tok_end++;
    }
    command_struct->cmd = calloc(tok_end - tok_start + 1, sizeof(char));
    strncpy(command_struct->cmd, tok_start, tok_end - tok_start);

    // Copy args.
    tok_end = command_string;
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
    Command * command;
    char * p;

    command_vec_size = 0;
    command_vec_cap = 1;
    command_vec = calloc(command_vec_cap + 1, sizeof(*command_vec));
    p = strtok(line, ";\n");

    while (p != NULL) {
        command = parse_command(p);
        if (command == NULL) {
            p = strtok(NULL, ";\n");
            continue;
        }
        command_vec[command_vec_size++] = command;
        command_vec[command_vec_size] = NULL;
        if (command_vec_size == command_vec_cap) {      // Expand command_vec
            command_vec_cap *= 2;
            command_vec = realloc(command_vec, sizeof(*command_vec) * (command_vec_cap + 1));
        }
        p = strtok(NULL, ";\n");
    }

    return command_vec;
}

int exec_commands(const CommandVec command_vec) {
    int ret;
    int i;

    for (i = 0; command_vec[i] != NULL; i++) {
        Command * command;
        pid_t pid;

        command = command_vec[i];
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Failed to fork\n");
            return -1;
        } else if (pid == 0) {
            exit(execvp(command->cmd, command->argv));
        } else {
            command->pid = pid;
        }
    }

    ret = 0;
    while (i-- > 0) {
        int pid;
        int status;
        pid = wait(&status);
        ret |= status;
    }

    return ret;
}

void free_command_vec(CommandVec command_vec) {
    int i;
    for (i = 0; command_vec[i] != NULL; i++) {
        Command * command = command_vec[i];
        int j;
        for (j = 0; j < command->argc; j++) {
            free(command->argv[j]);
        }
        free(command->argv);
        free(command->cmd);
        free(command);
    }
    free(command_vec);
}

int main(int argc, char * argv[]) {
    FILE * p_infile;
    char * line;
    enum MODE { BATCH, INTERACTIVE } mode;

    mode = (argc == 1) ? INTERACTIVE : BATCH;

    p_infile = NULL;
    if (mode == INTERACTIVE) {
        p_infile = stdin;
    } else if (mode == BATCH) {
        p_infile = fopen(argv[1], "r");
    }

    if (p_infile == NULL) {
        fprintf(stderr, "Failed to open input stream.\n");
        exit(1);
    }

    while (true) {
        CommandVec cmd_vec;

        if (mode == INTERACTIVE) {
            printf("prompt> ");
            fflush(stdout);
        }
        line = read_line(p_infile);
        if (line == NULL || strcmp(line, "quit\n") == 0) {
            break;
        }

        cmd_vec = parse_line(line);
        exec_commands(cmd_vec);

        free(line);
        free_command_vec(cmd_vec);
    }

    return 0;
}

