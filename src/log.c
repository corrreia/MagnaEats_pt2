#include "../include/log.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

FILE * log_file;

void createLogFile(char * filename){
    log_file =  fopen (filename, "w");
}

void writeRead(int op){
    getTime();
    fprintf(log_file,"%s %d\n","read",op);
}

void writeStop(){
    getTime();
    fprintf(log_file,"%s\n","stop");
}

void writeOp(int client_id,int restaurant_id,char * dish){
    getTime(log_file);
    fprintf(log_file,"%s %d %d %s\n","request", client_id, restaurant_id,dish);

}
void writeHelp(){
    getTime(log_file);
    fprintf(log_file,"%s\n","help");
}

void getTime(){
    struct timespec timer;
    clock_gettime( CLOCK_REALTIME, &timer);
    char t [40];
    strftime(t,40,"%F %X",localtime(&timer.tv_sec));

    fprintf(log_file,"%s.%ld ",t,timer.tv_nsec/1000000);
}

void closeFile(){
    closeFile(log_file);
}