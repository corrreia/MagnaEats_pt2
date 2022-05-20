#include "../include/mesignal.h"

#include "../include/memory.h"
#include "../include/synchronization.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "../include/main.h"

extern struct main_data* data;
extern struct communication_buffers* buffers;
extern struct semaphores* sems;

//CTRL-C
void ctrlC(){
    stop_execution(data, buffers, sems); //Arrajar maneira de saber o op_counter ??
    
    exit(1);
}

void catchSignal(){  // para a captura do CTRL-C na funçao main
    signal(SIGINT, ctrlC);
}

void ignoreSignal(){  //para a captura do CTRL-C nos processos filhos
    signal(SIGINT, SIG_IGN);
}

//Alarme

void alarm_handler() {
    signal(SIGALRM, alarm_handler);

    semaphore_mutex_lock(sems->results_mutex); //Bloqueia o mutex
    for(int i = 0; i < data->max_ops; i++) {

        if ((data->results+i) != NULL && (data->results+i)->status == 'C') {
            printf("request:%d status:C start: %ld restaurante:%d rest_time: %ld driver:%d driver_time: %ld client:%d client_end_time: %ld \n", 
            i, 
            (data->results+i)-> start_time.tv_sec, 
            (data->results+i)-> receiving_rest, 
            (data->results+i)-> rest_time.tv_sec, 
            (data->results+i)-> receiving_driver,
            (data->results+i)-> driver_time.tv_sec,
            (data->results+i)-> receiving_client, 
            (data->results+i)-> client_end_time.tv_sec);
        }  else {
            printf("request: %d status: %c \n", i, (data->results+i)->status);
        }
    }
    semaphore_mutex_unlock(sems->results_mutex); //Desbloqueia o mutex

}

void createAlarm(int alarmTime){
    signal(SIGALRM, alarm_handler); //Cria o alarme

    struct itimerval timer;                 //Cria o timer
    timer.it_value.tv_sec = alarmTime;      //Tempo de inicio
    timer.it_value.tv_usec = 0;             
    timer.it_interval.tv_sec = alarmTime;   //Tempo de intervalo 
    timer.it_interval.tv_usec = 0;          

    setitimer(ITIMER_REAL, &timer, NULL);  //Inicia o timer
}


