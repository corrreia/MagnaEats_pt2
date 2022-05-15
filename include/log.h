#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include <stdio.h>

FILE * createLogFile(char * filename);

void writeOperation(char * operation, FILE * log_file);

void closeFile(FILE * log_file);


#endif