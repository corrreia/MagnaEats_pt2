#include "../include/log.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

FILE * createLogFile(char * filename){
    return fopen (filename, "w");
}

void writeOperation(char * operation,FILE * log_file){
    char * ptr = strtok(operation, " ");

    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );


    if(strstr(operation,"stop") !=0){
        fprintf(log_file,"%s",asctime(info));
        fputs(" stop ",log_file);
    }
    else if(strstr(operation,"read") !=0){
        fprintf(log_file,"%s",asctime(info));
        fputs(" read ",log_file);
        fprintf(log_file,"%s",ptr);
    }
    else if(strstr(operation,"op") !=0){
        fprintf(log_file,"%s",asctime(info));
        fputs(" Op ",log_file);
    }
    else if(strstr(operation,"help") !=0){
        fprintf(log_file,"%s",asctime(info));
        fputs("help ",log_file);
    }
}

void closeFile(FILE * log_file){
    closeFile(log_file);
}