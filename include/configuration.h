#ifndef CONFIGURATION_H_GUARD
#define CONFIGURATION_H_GUARD

#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "main.h"
#include "memory.h"
#include "mesignal.h"


/* Função que lê os argumentos de um ficheiro dada em fileName
*/
void lerFicheiro(struct main_data* data, char fileName[]);

#endif
