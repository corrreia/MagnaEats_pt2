#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/main.h"
#include "../include/process.h"
#include "../include/driver.h"
#include "../include/client.h"
#include "../include/restaurant.h"


/* Função que inicia um novo processo restaurante através da função fork do SO. O novo
* processo irá executar a função execute_restaurant respetiva, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_restaurant(int restaurant_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid;
    if((pid=fork())== 0){
        int t = execute_restaurant(restaurant_id,buffers,data,sems);
        exit(t);
    }
    else if(pid == -1){
        perror("Error in fork");
        exit(1);
    }else{
        return pid;
    }
}

/* Função que inicia um novo processo motorista através da função fork do SO. O novo
* processo irá executar a função execute_driver, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_driver(int driver_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid;
    if((pid=fork())== 0){
        int t = execute_driver(driver_id,buffers,data,sems);
        exit(t);
    }
    else if(pid == -1){
        perror("Error in fork");
        exit(1);
    }else{
        return pid;
    }
}


/* Função que inicia um novo processo cliente através da função fork do SO. O novo
* processo irá executar a função execute_client, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_client(int client_id, struct communication_buffers* buffers, struct main_data* data,  struct semaphores* sems){
    int pid;
    if((pid=fork())== 0){
        int t = execute_client(client_id,buffers,data,sems);
        exit(t);
    }else if(pid == -1){
        perror("Error in fork");
        exit(1);
    }else{
        return pid;
    }
}


/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id){
    int result;
    waitpid(process_id,&result,0);
    if(WIFEXITED(result)){
        return WEXITSTATUS(result);
    }
}