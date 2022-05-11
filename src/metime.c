#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/metime.h"

void registarTempo(struct timespec* t){
    if( clock_gettime( CLOCK_REALTIME, t) == -1 ) {
        exit(1);
    }
}