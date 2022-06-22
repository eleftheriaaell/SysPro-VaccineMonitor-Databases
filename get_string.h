#ifndef GET_STRING_H
#define GET_STRING_H

#include "extra_lists.h"

void get_record(record* head, char* fileName);

void push_record(record* head, record* node, char** Command);

char** get_command(char* command, int num);

char** get_date(char* date);

#endif