#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/main.h"
#include "../include/log.h"
#include "../include/memory.h"
#include "../include/process.h"
#include "../include/configuration.h"
#include "../include/synchronization.h"
#include "../include/metime.h"
#include "../include/stats.h"


int main(int argc, char *argv[]) {
    //init data structures
    struct main_data* data = create_dynamic_memory(sizeof(struct main_data));
    struct communication_buffers* buffers =
    create_dynamic_memory(sizeof(struct communication_buffers));
    buffers->main_rest = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->rest_driv = create_dynamic_memory(sizeof(struct circular_buffer));
    buffers->driv_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    // init semaphore data structure
    struct semaphores* sems = create_dynamic_memory(sizeof(struct semaphores));
    sems->main_rest = create_dynamic_memory(sizeof(struct prodcons));
    sems->rest_driv = create_dynamic_memory(sizeof(struct prodcons));
    sems->driv_cli = create_dynamic_memory(sizeof(struct prodcons));
    //execute main code
    main_args(argc, argv, data);
    create_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, buffers);
    create_semaphores(data, sems);
    launch_processes(buffers, data, sems);
    user_interaction(buffers, data, sems);
    //release memory before terminating
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_rest);
    destroy_dynamic_memory(buffers->rest_driv);
    destroy_dynamic_memory(buffers->driv_cli);
    destroy_dynamic_memory(buffers);
    destroy_dynamic_memory(sems->main_rest);
    destroy_dynamic_memory(sems->rest_driv);
    destroy_dynamic_memory(sems->driv_cli);
    destroy_dynamic_memory(sems);
}

/* Função que lê os argumentos da aplicação, nomeadamente o número
* máximo de operações, o tamanho dos buffers de memória partilhada
* usados para comunicação, e o número de clientes, de motoristas e de
* restaurantes. Guarda esta informação nos campos apropriados da
* estrutura main_data.
*/
void main_args(int argc, char* argv[], struct main_data* data){  
    if(argc != 1){
        lerFicheiro(data,argv[1]);
    }
    else{
        printf("Uso: magnaeats nomeDoFicheiroDeConfiguração\n");
        printf("Exemplo: ./bin/magnaeats path/okay.h \n");
        exit(1);
    }
}

/* Função que reserva a memória dinâmica necessária para a execução
* do MAGNAEATS, nomeadamente para os arrays *_pids e *_stats da estrutura 
* main_data. Para tal, pode ser usada a função create_dynamic_memory.
*/
void create_dynamic_memory_buffers(struct main_data* data){
    data -> restaurant_pids = create_dynamic_memory(data->n_restaurants*sizeof(int)); 
    data -> client_pids = create_dynamic_memory(data->n_clients*sizeof(int)); 
    data -> driver_pids = create_dynamic_memory(data->n_drivers*sizeof(int)); 

    data -> restaurant_stats = create_dynamic_memory(data->n_restaurants*sizeof(int)); 
    data -> client_stats = create_dynamic_memory(data->n_clients*sizeof(int)); 
    data -> driver_stats = create_dynamic_memory(data->n_drivers*sizeof(int));
}

/* Função que reserva a memória partilhada necessária para a execução do
* MAGNAEATS. É necessário reservar memória partilhada para todos os buffers da
* estrutura communication_buffers, incluindo os buffers em si e respetivos
* pointers, assim como para o array data->results e variável data->terminate.
* Para tal, pode ser usada a função create_shared_memory.
*/
void create_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers){
    buffers -> main_rest -> buffer = create_shared_memory(STR_SHM_MAIN_REST_BUFFER,data -> buffers_size   * sizeof(struct operation)); 
    buffers -> rest_driv -> buffer = create_shared_memory(STR_SHM_REST_DRIVER_BUFFER,data -> buffers_size * sizeof(struct operation));
    buffers -> driv_cli -> buffer = create_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER,data -> buffers_size* sizeof(struct operation));

    buffers -> main_rest -> ptrs = create_shared_memory(STR_SHM_MAIN_REST_PTR,data -> buffers_size *sizeof(int*)); 
    buffers -> rest_driv -> ptrs = create_shared_memory(STR_SHM_REST_DRIVER_PTR,sizeof(struct pointers));
    buffers -> driv_cli -> ptrs = create_shared_memory(STR_SHM_DRIVER_CLIENT_PTR,data -> buffers_size *sizeof(int*));


    data -> results = create_shared_memory(STR_SHM_RESULTS,data -> max_ops * (sizeof(struct operation)));
    data -> terminate = create_shared_memory(STR_SHM_TERMINATE,sizeof(int));
}


/* Função que inicia os processos dos restuarantes, motoristas e clientes. Para tal, pode
* usar a função launch_process, guardando os pids resultantes nos arrays respetivos
* da estrutura data.
*/
void launch_processes(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    for(int i = 0; i< data->n_restaurants;i++){
        data->driver_pids[i] = launch_restaurant(i,buffers,data,sems);       
    }
    for(int i = 0; i< data->n_drivers;i++){
        data->client_pids[i] = launch_driver(i,buffers,data,sems);
    }
    for(int i = 0; i< data->n_clients;i++){
        data->restaurant_pids[i] = launch_client(i,buffers,data,sems);
    }
}

/* Função que faz interação do utilizador, podendo receber 4 comandos:
* op - cria uma nova operação, através da função create_request
* read - verifica o estado de uma operação através da função read_answer
* stop - termina o execução do sovaccines através da função stop_execution
* help - imprime informação sobre os comandos disponiveis
*/
void user_interaction(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int op_counter = 0;
    char pedido [40];

    printf("Operações possiveis:\n");
    printf("        op client restaurant dish - criar um novo pedido\n");
    printf("        read id - consultar o estado de um pedido\n");
    printf("        stop - termina a execução do magnaeats.\n");
    printf("        help - imprime informação sobre as ações disponíveis.\n");

    while(1){
        printf("Introduzir comando:\n");
        scanf("%s", pedido);


        if(strstr(pedido,"stop") !=0){
            //tive que retirar do sitio para aqui pois estava a escrever errado
            setOpCounter(op_counter);
            writeStop();
            stop_execution(data,buffers,sems);
            exit(1);
        }
        else if(strstr(pedido,"read") !=0){
            read_status(data,sems);

        }
        else if(strstr(pedido,"op") !=0){
            create_request(&op_counter,buffers,data,sems);
        }
        else if(strstr(pedido,"help") !=0){
            printf("Operações possiveis:\n");
            printf("        op client restaurant dish - criar um novo pedido\n");
            printf("        read id - consultar o estado de um pedido\n");
            printf("        stop - termina a execução do magnaeats.\n");
            printf("        help - imprime informação sobre as ações disponíveis.\n");
            writeHelp();
        }
        else{
            printf("Ação não reconhecida, insira 'help' para assistência.\n");
        }
    }

}

/* Se o limite de operações ainda não tiver sido atingido, cria uma nova
* operação identificada pelo valor atual de op_counter, escrevendo a mesma
* no buffer de memória partilhada entre main e clientes e efetuando a 
* necessária sincronização antes e depois de escrever. Imprime o id da
* operação e incrementa o contador de operações op_counter.
*/
void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int client_ID;
    int restaurant_ID;
    char *dish = (char*) malloc(20);
    scanf("%d",&client_ID);
    scanf("%d",&restaurant_ID);
    scanf("%s",dish);

    writeOp(client_ID,restaurant_ID,dish);

    if(*op_counter < data->max_ops){ 
        struct operation op = {
            *op_counter,
            client_ID,
            restaurant_ID,
            NULL,
            'I',
            0,
            0,
            0,
            0,//colocar startingtime
            0,
            0,
            0
        };//falta meter tempos
        registarTempo(&op.start_time);
        op.requested_dish = (char*) create_dynamic_memory(20);
        strcpy(op.requested_dish,dish);

        printf("O pedido #%d foi criado!\n", *op_counter);

        memcpy(&data->results[*op_counter],&op,sizeof(struct operation));


        produce_begin(sems->main_rest);   
        write_main_rest_buffer(buffers -> main_rest,data -> buffers_size,&op);
        produce_end(sems->main_rest);
        printf("okay\n");
        

        *op_counter = *op_counter + 1;
    }
    else{
        printf("número máximo de operações excedido\n");
    }
    free(dish);
}

/* Função que lê um id de operação do utilizador e verifica se a mesma
* é valida. Em caso afirmativo,
* imprime informação da mesma, nomeadamente o seu estado, o id do cliente
* que fez o pedido, o id do restaurante requisitado, o nome do prato pedido
* e os ids do restaurante, motorista, e cliente que a receberam e processaram.
*/
void read_status(struct main_data* data, struct semaphores* sems){
    int id = 0;
    scanf("%d",&id);
    writeRead(id);
    if(data->results[id].requested_dish == NULL){
        printf("O id inserido não é válido!\n");
    }
    else{
        printf("Pedido %d com estado %c requisitado pelo cliente %d ao restaurante %d com o prato %s, foi tratado pelo restaurante %d, encaminhado pelo motorista %d, e enviado ao cliente %d!\n",
        id,
        data->results[id].status,
        data->results[id].requesting_client,
        data->results[id].requested_rest,
        data->results[id].requested_dish,
        data->results[id].receiving_rest,
        data->results[id].receiving_driver,
        data->results[id].receiving_client);
    }
}

/* Função que termina a execução do programa sovaccines. Deve começar por 
* afetar a flag data->terminate com o valor 1. De seguida, e por esta
* ordem, deve acordar processos adormecidos, esperar que terminem a sua 
* execução, escrever as estatisticas finais do programa, e por fim libertar
* os semáforos e zonas de memória partilhada e dinâmica previamente 
*reservadas. Para tal, pode usar as outras funções auxiliares do main.h.
*/
void stop_execution(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems){
    //não faz sentido pois temos de abrir um ficheiro e rescrever o que já foi escrito
    *data->terminate = 1;
    wakeup_processes(data,sems);
    wait_processes(data);
    write_statistics(data);
    closeStatsFile();
    destroy_semaphores(sems);
    destroy_memory_buffers(data,buffers);
}

/* Função que espera que todos os processos previamente iniciados terminem,
* incluindo restaurantes, motoristas e clientes. Para tal, pode usar a função 
* wait_process do process.h.
*/
void wait_processes(struct main_data* data){
    for(int i = 0; i< data->n_clients;i++){
        data -> client_stats[i] = wait_process((data->client_pids[i]));
    }
    for(int i = 0; i< data->n_drivers;i++){
       data -> driver_stats[i] = wait_process((data->driver_pids[i]));
    }
    for(int i = 0; i< data->n_restaurants;i++){
        data -> restaurant_stats[i] = wait_process((data->restaurant_pids[i]));       
    }
}

/* Função que imprime as estatisticas finais do MAGNAEATS, nomeadamente quantas
* operações foram processadas por cada restaurante, motorista e cliente.
*/
void write_statistics(struct main_data* data){
    for(int i = 0; i< data->n_restaurants;i++){
        printf("Restaurante %d preparou %d pedidos!\n",i,data->restaurant_stats[i]);     
    }
    for(int i = 0; i< data->n_drivers;i++){
        printf("Motorista %d entregou %d pedidos!\n",i,(data->driver_stats[i]));
    }
    for(int i = 0; i< data->n_clients;i++){
       printf("Cliente %d recebeu %d pedidos!\n",i,(data->client_stats[i]));
    }
    doStats(data);
}

/* Função que liberta todos os buffers de memória dinâmica e partilhada previamente
* reservados na estrutura data.
*/
void destroy_memory_buffers(struct main_data* data, struct communication_buffers* buffers){
    destroy_dynamic_memory(data->restaurant_pids);
    destroy_dynamic_memory(data->client_pids);
    destroy_dynamic_memory(data->driver_pids);

    destroy_dynamic_memory(data->restaurant_stats);
    destroy_dynamic_memory(data->client_stats);
    destroy_dynamic_memory(data->driver_stats);
    
    for(int i = 0; i< data->max_ops;i++){
        if(data->results[i].requested_dish != NULL){
            destroy_dynamic_memory(data->results[i].requested_dish);
        }
    }

    destroy_shared_memory(STR_SHM_MAIN_REST_BUFFER,buffers -> main_rest -> buffer, data -> buffers_size   * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_REST_DRIVER_BUFFER,buffers -> rest_driv -> buffer,data -> buffers_size   * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER,buffers -> driv_cli -> buffer,data -> buffers_size   * sizeof(struct operation));

    destroy_shared_memory(STR_SHM_MAIN_REST_PTR, buffers->main_rest->ptrs,data -> buffers_size   * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_REST_DRIVER_PTR, buffers->rest_driv->ptrs,sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, buffers->driv_cli->ptrs,data -> buffers_size   * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_RESULTS,data -> results,data -> max_ops * (sizeof(struct operation)));
    destroy_shared_memory(STR_SHM_TERMINATE,data ->terminate,sizeof(int));
}

/* Função que inicializa os semáforos da estrutura semaphores. Semáforos
* *_full devem ser inicializados com valor 0, semáforos *_empty com valor
* igual ao tamanho dos buffers de memória partilhada, e os *_mutex com valor
* igual a 1. Para tal pode ser usada a função semaphore_create.
*/
void create_semaphores(struct main_data* data, struct semaphores* sems){
    sems-> main_rest -> full = semaphore_create(STR_SEM_MAIN_REST_FULL,0);
    sems-> main_rest -> empty = semaphore_create(STR_SEM_MAIN_REST_EMPTY,data->buffers_size);
    sems-> main_rest -> mutex = semaphore_create(STR_SEM_MAIN_REST_MUTEX,1);

    sems-> rest_driv -> full = semaphore_create(STR_SEM_REST_DRIV_FULL,0);
    sems-> rest_driv -> empty = semaphore_create(STR_SEM_REST_DRIV_EMPTY,data->buffers_size);
    sems-> rest_driv -> mutex = semaphore_create(STR_SEM_REST_DRIV_MUTEX,1);

    sems-> driv_cli -> full = semaphore_create(STR_SEM_DRIV_CLI_FULL,0);
    sems-> driv_cli -> empty = semaphore_create(STR_SEM_DRIV_CLI_EMPTY,data->buffers_size);
    sems-> driv_cli -> mutex = semaphore_create(STR_SEM_DRIV_CLI_MUTEX,1);

    //Falta criar um no results
}

/* Função que acorda todos os processos adormecidos em semáforos, para que
* estes percebam que foi dada ordem de terminação do programa. Para tal,
* pode ser usada a função produce_end sobre todos os conjuntos de semáforos
* onde possam estar processos adormecidos e um número de vezes igual ao 
* máximo de processos que possam lá estar.
*/
void wakeup_processes(struct main_data* data, struct semaphores* sems){
    //não funciona para n_restaurants, n_drivers e n_clients
    for(int i = 0; i<data->n_restaurants;i++){
        produce_end(sems-> main_rest);
        produce_end(sems-> rest_driv);
        produce_end(sems -> driv_cli);
    }
    for(int i = 0; i<data->n_drivers;i++){
        produce_end(sems-> main_rest);
        produce_end(sems-> rest_driv);
        produce_end(sems -> driv_cli);
    }
    for(int i = 0; i<data->n_clients;i++){
        produce_end(sems-> main_rest);
        produce_end(sems-> rest_driv);
        produce_end(sems -> driv_cli);
    }
}

/* Função que liberta todos os semáforos da estrutura semaphores.
*/
void destroy_semaphores(struct semaphores* sems){
    semaphore_destroy(STR_SEM_MAIN_REST_FULL,sems-> main_rest -> full);
    semaphore_destroy(STR_SEM_MAIN_REST_EMPTY,sems-> main_rest -> empty);
    semaphore_destroy(STR_SEM_MAIN_REST_MUTEX,sems-> main_rest -> mutex);

    semaphore_destroy(STR_SEM_REST_DRIV_FULL,sems-> rest_driv -> full);
    semaphore_destroy(STR_SEM_REST_DRIV_EMPTY,sems-> rest_driv -> empty);
    semaphore_destroy(STR_SEM_REST_DRIV_MUTEX,sems-> rest_driv -> mutex);

    semaphore_destroy(STR_SEM_DRIV_CLI_FULL,sems-> driv_cli -> full);
    semaphore_destroy(STR_SEM_DRIV_CLI_EMPTY,sems-> driv_cli -> empty);
    semaphore_destroy(STR_SEM_DRIV_CLI_MUTEX,sems-> driv_cli -> mutex);

    //Falta retirar um no results
}