#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/configuration.h"
#include "../include/memory.h"
#include "../include/main.h"
#include "../include/log.h"
#include "../include/stats.h"
#include "../include/mesignal.h"


void lerFicheiro(struct main_data* data, char fileName[]){

    //Open file
    FILE *in_file  = fopen(fileName, "r"); // read only 

    fscanf(in_file,"%d", &data -> max_ops); // read max_ops
    fscanf(in_file,"%d", &data -> buffers_size); // read buffers_size
    fscanf(in_file,"%d", &data -> n_restaurants); // read n_restaurants
    fscanf(in_file,"%d", &data -> n_drivers); // read n_drivers
    fscanf(in_file,"%d", &data -> n_clients); // read n_clients

    char logs_file_name [30];
    fscanf(in_file,"%s", logs_file_name);
    createLogFile(logs_file_name);
 

    char stats_file_name [30];  //stats file name
    fscanf(in_file,"%s",  stats_file_name);
    createStatsFile(stats_file_name);
    
    int time = 5; //default value for time is 5
    fscanf(in_file,"%d", &time); // read time from file
    createAlarm(time);  //create alarm with time	

    catchSignal(); //catch CTRL-C signal

    //close file
    fclose(in_file);
}