#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void main(){
struct timespec inicio, fim;
double dif;
if( clock_gettime( CLOCK_REALTIME, &inicio) == -1 ) {
 perror( "clock gettime" );
 exit( EXIT_FAILURE );
}
sleep(1);
if( clock_gettime( CLOCK_REALTIME, &fim) == -1 ) {
 perror( "clock gettime" );
 exit( EXIT_FAILURE );
}
dif = ( fim.tv_sec - inicio.tv_sec )
 + ( fim.tv_nsec - inicio.tv_nsec )
 / 1000000000L;
printf( "%lf\n", dif );
}