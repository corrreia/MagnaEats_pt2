#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include <stdio.h>

FILE * createLogFile(char * filename);

void writeRead(int op, FILE * log_file);

void writeStop(FILE * log_file);

void writeOp(int client_id,int restaurant_id,char * dish, FILE * log_file);

void writeHelp(FILE * log_file);

void getTime(FILE * log_file);

void closeFile(FILE * log_file);


#endif