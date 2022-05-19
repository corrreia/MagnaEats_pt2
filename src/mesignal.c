#include "../include/mesignal.h"

#include "../include/main.h"
#include "../include/memory.h"
#include "../include/synchronization.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "../include/globals.h"
//[*]->[*]->createdynamicfodasse();
//CTRL-C
void ctrlC(){
    stop_execution(data, buffers, sems);
    //printf("AHHHHHH EU TO MALUCO\n");

    //printf("%s",data -> results[0].requested_dish);

    exit(1);
}

void createSignal (){
    // data1 = data;
    // buffers1 = buffers;
    // sems1 = sems;
    signal(SIGINT, ctrlC);
}


void alarmHandler(){
    
}
//Alarme
void createAlarm(int alarmTime){
    struct sigaction sa;
    sa.sa_handler = alarmHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    alarm(alarmTime);
}


