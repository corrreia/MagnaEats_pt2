#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void main(){
    struct timespec t;
    clock_gettime(CLOCK_REALTIME,&t);
    time_t rawtime = t.tv_sec;
    struct tm *info = localtime( &rawtime);
    //time( &rawtime );

    printf("Current local time and date: %s", asctime(info));
    /*
    struct timespec t;
    clock_gettime(CLOCK_REALTIME,&t);
    time_t h = t.tv_sec;
    printf("%ld\n",t.tv_sec);
    printf("%s\n",ctime(t.tv_sec));
    */

}