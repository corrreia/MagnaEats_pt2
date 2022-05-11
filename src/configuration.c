
#include <stdio.h>
#include <stdlib.h>
#include "../include/configuration.h"
#include "../include/main.h"


void lerFicheiro(struct main_data* data, char fileName[]){

    //Open file
    FILE *in_file  = fopen(fileName, "r"); // read only 

    fscanf(in_file,"%d",  &data -> max_ops);
    fscanf(in_file,"%d",  &data -> buffers_size);
    fscanf(in_file,"%d",  &data -> n_restaurants);
    fscanf(in_file,"%d",  &data -> n_drivers);
    fscanf(in_file,"%d",  &data -> n_clients);
    fscanf(in_file,"%s",  data -> log_filename);
    fscanf(in_file,"%s",  data -> statistics_filename);
    fscanf(in_file,"%d",  &data -> alarm_time);

    //close file
    fclose(in_file);
}