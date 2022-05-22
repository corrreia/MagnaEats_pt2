#ifndef MESIGNAL_H_GUARD
#define MESIGNAL_H_GUARD

#include "main.h"
#include "memory.h"
#include <semaphore.h>

/**
 * Função que executa quando é capturado um SIGINT(ctrl+C)
 * 
 */
void ctrlC();

/**
 * Função que apanha o sinal CTRL-C para que o programa seja interrompido
 * 
 */
void catchSignal();


/**
 * Função que ignora um sinal de SIGINT nos filhos.
 * 
 */
void ignoreSignal();

/**
 * Função que imprime na consola as stats do programa.
 * 
 */
void alarm_handler(struct operation* results);


/**
 * Função que cria um novo alarme com o tempo até disparar de alarmTime.
 * 
 * @param alarmTime - tempo necessário para o alarme disparar
 */
void createAlarm(int alarmTime);

#endif
