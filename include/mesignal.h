#ifndef MESIGNAL_H_GUARD
#define MESIGNAL_H_GUARD

#include "main.h"
#include "memory.h"
#include <semaphore.h>

void ctrlC();

void catchSignal();

void ignoreSignal();

void createAlarm(int alarmTime);

void doAlarm(struct operation* results);

#endif