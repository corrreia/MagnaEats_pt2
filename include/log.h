#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include <stdio.h>

/**
 * Função que abre um ficheiro com o nome filename
 * 
 */
void createLogFile(char * filename);

/**
 * Função que escreve uma operação no ficheiro de log
 * 
 * @param op - nº da operação a escrever
 */
void writeRead(int op);

/**
 * Função que escreve uma operação de stop num ficheiro de log
 */
void writeStop();

/**
 * Função que escreve uma operação de pedido no ficheiro de log
 * 
 * @param client_id - nº do cliente que fez o pedido
 * @param restaurant_id - nº do restaurante que recebeu o pedido
 * @param dish - nome do prato que foi pedido
 */
void writeOp(int client_id,int restaurant_id,char * dish);

/**
 * Função que escreve uma operação de help num no ficheiro de log
 * 
 */
void writeHelp();

/**
 * Função que escreve o tempo atual no ficheiro de log
 */
void getTime();

/**
 * Função que fecha o ficheiro de log
 * 
 */
void closeFile();


#endif