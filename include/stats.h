#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "main.h"

void createStatsFile(char * fileName);

void setOpCounter(int counter);

void doStats(struct main_data* data);

void closeStatsFile();

#endif