#ifndef CONFIGURATION_H_GUARD
#define CONFIGURATION_H_GUARD

#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "main.h"
#include "memory.h"
#include "mesignal.h"

void lerFicheiro(struct main_data* data, char fileName[]);

#endif
