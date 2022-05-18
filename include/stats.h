#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "main.h"

void createStatsFile(char * fileName);

void doStats(struct main_data* data, int op_counter);

void closeStatsFile();

#endif