#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "../include/client.h"
#include "../include/main.h"
#include "../include/memory.h"
#include "../include/metime.h"

/* Função principal de um Cliente. Deve executar um ciclo infinito onde em 
* cada iteração lê uma operação da main e se for dirijida ao cliente client_id 
* e data->terminate ainda for igual a 0, processa-a e guarda o resultado no histórico
* de operações da Main. Operações com id igual a -1 são ignoradas (op inválida) e se
* data->terminate for igual a 1 é porque foi dada ordem de terminação do programa,
* portanto deve-se fazer return do o número de operações processadas. Para efetuar
* estes passos, pode usar os outros métodos auxiliares definidos em client.h. 
*/
int execute_client(int client_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int op_counter = 0;
    while(*(data -> terminate) == 0){
        struct operation* op = malloc(sizeof(struct operation));
      
        client_get_operation(op, client_id, buffers,data,sems);

        if(op -> id != -1 && op->status == 'D'){
            printf("Cliente %d recebeu pedido! %d#\n",client_id, op -> id);
            client_process_operation(op, client_id, data, &op_counter,sems);
        }
        free(op);
    }
    return op_counter;
}

/* Função que lê uma operação do buffer de memória partilhada entre os motoristas
* e clientes que seja direcionada a client_id. Antes de tentar ler a operação, deve
* verificar se data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente da função.
*/
void client_get_operation(struct operation* op, int client_id, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if(*(data -> terminate) == 1) return;
    consume_begin(sems->driv_cli);
    read_driver_client_buffer(buffers->driv_cli,client_id,data -> buffers_size,op);
    consume_end(sems->driv_cli);
}

/* Função que processa uma operação, alterando o seu campo receiving_client para o id
* passado como argumento, alterando o estado da mesma para 'C' (client), e 
* incrementando o contador de operações. Atualiza também a operação na estrutura data.
*/
void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter, struct semaphores* sems){
    op -> receiving_client = client_id;
    op -> status = 'C';
    registarTempo(&op->client_end_time);
    //Será que é o mutex do results??
    *counter = *counter + 1;
    data -> results[op -> id] = *op;
}