#ifndef METIME_H_GUARD
#define METIME_H_GUARD

#include <time.h>

/**
 *Função que dado uma estrutura timespec regista o tempo atual
 * 
 * @param sitio - estrutura para escrever o tempo atual
 */
void registarTempo(struct timespec* sitio);

#endif