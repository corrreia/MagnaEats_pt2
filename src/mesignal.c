#include "../include/mesignal.h"

#include "../include/main.h"
#include "../include/memory.h"
#include "../include/synchronization.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "../include/globals.h"

//CTRL-C
void ctrlC(){
    stop_execution(data, buffers, sems);
    //Arrajar maneira de saber o op_counter 
    exit(1);
}

void createSignal (){
    signal(SIGINT, ctrlC);
}

void ignoreSignal(){
    signal(SIGINT, SIG_IGN);
}


void alarmHandler(){
    printf("AHHH EU TO MALUCO\n");
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


