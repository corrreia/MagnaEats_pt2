#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include <stdio.h>

void createLogFile(char * filename);

void writeRead(int op);

void writeStop();

void writeOp(int client_id,int restaurant_id,char * dish);

void writeHelp();

void getTime();

void closeFile();


#endif