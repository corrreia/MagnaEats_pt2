#include "../include/log.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

FILE * createLogFile(char * filename){
    return fopen (filename, "w");
}

void writeRead(int op, FILE * log_file){
    getTime(log_file);
    fprintf(log_file,"%s %d\n","read",op);
}

void writeStop(FILE * log_file){
    getTime(log_file);
    fprintf(log_file,"%s\n","stop");
}

void writeOp(int client_id,int restaurant_id,char * dish, FILE * log_file){
    getTime(log_file);
    fprintf(log_file,"%s %d %d %s\n","op", client_id, restaurant_id,dish);

}
void writeHelp(FILE * log_file){
    getTime(log_file);
    fprintf(log_file,"%s\n","help");
}

void getTime(FILE * log_file){
    struct timespec timer;
    clock_gettime( CLOCK_REALTIME, &timer);
    char t [40];
    strftime(t,40,"%F %X",localtime(&timer.tv_sec));

    fprintf(log_file,"%s.%ld ",t,timer.tv_nsec/1000000);
}

void closeFile(FILE * log_file){
    closeFile(log_file);
}