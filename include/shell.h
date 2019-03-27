//
// Created by 김지훈 on 2019-03-25.
//

#pragma once
#ifndef INC_1901_OPERATINGSYSTEMS_ELE3021_SHELL_H
#define INC_1901_OPERATINGSYSTEMS_ELE3021_SHELL_H

// Struct representing a single command.
// Commands contained in a same line are linked.
struct CommandNode {
    char * cmd;
    char ** argv;
    struct CommandNode * p_nxt;
};

typedef struct CommandNode  CommandNode;
typedef CommandNode *       CommandList;

char * read_line(FILE * infile);
CommandList parse_line(char * line);
int exec_command(const CommandNode * command);

#endif //INC_1901_OPERATINGSYSTEMS_ELE3021_SHELL_H
