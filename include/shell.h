//
// Created by 김지훈 on 2019-03-25.
//

#pragma once
#ifndef INC_1901_OPERATINGSYSTEMS_ELE3021_SHELL_H
#define INC_1901_OPERATINGSYSTEMS_ELE3021_SHELL_H

typedef struct {

} Command;

char * read_line(FILE * infile);
char ** parse_line(char *);

#endif //INC_1901_OPERATINGSYSTEMS_ELE3021_SHELL_H
