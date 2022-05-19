#include <stdio.h>
#include <stdlib.h>
#include "../include/memory.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include "../include/synchronization.h"

/* Função que reserva uma zona de memória partilhada com tamanho indicado
* por size e nome name, preenche essa zona de memória com o valor 0, e 
* retorna um apontador para a mesma. Pode concatenar o resultado da função
* getuid() a name, para tornar o nome único para o processo.
*/
void* create_shared_memory(char* name, int size)
{

    int* ptr;
    int ret;
    int fd;
    fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ret = ftruncate (fd, size);
    ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //finf errors
    if (ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    //erros in ret
    if (ret == -1)
    {
        perror("ftruncate");
        exit(1);
    }
    
    bzero(ptr,size);
    return ptr;
}


/* Função que reserva uma zona de memória dinâmica com tamanho indicado
* por size, preenche essa zona de memória com o valor 0, e retorna um 
* apontador para a mesma.
*/
void* create_dynamic_memory(int size){
    void *ptr = malloc(size);
    bzero(ptr,size);
    return ptr;
}

/* Função que liberta uma zona de memória partilhada previamente reservada.
*/
void destroy_shared_memory(char* name, void* ptr, int size)
{
    munmap(ptr, size);
    shm_unlink(name);
}


/* Função que liberta uma zona de memória dinâmica previamente reservada.
*/
void destroy_dynamic_memory(void* ptr){
    free(ptr);
}


/* Função que escreve uma operação no buffer de memória partilhada entre a Main
* e os restaurantes. A operação deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_main_rest_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    int n;
    for(n = 0; n < buffer_size; n++){ 
        if(buffer -> ptrs[n] == 0){
            memcpy(&buffer -> buffer[n],op,sizeof(struct operation));
            int i = 1;
            memcpy(&buffer -> ptrs[n],&i,sizeof(int));
            return;
        }
    }
}


/* Função que escreve uma operação no buffer de memória partilhada entre os restaurantes
* e os motoristas. A operação deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
        if(((buffer -> ptrs ->in + 1) % buffer_size) != buffer -> ptrs -> out){
            memcpy(&buffer ->buffer[buffer -> ptrs -> out],op,sizeof(struct operation));
            buffer -> ptrs ->in = (buffer -> ptrs ->in +1) % buffer_size;
        }
}


/* Função que escreve uma operação no buffer de memória partilhada entre os motoristas
* e os clientes. A operação deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_driver_client_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    int n;
    for(n = 0; n < buffer_size; n++){
        if(buffer -> ptrs[n] == 0){
            int i = 1;
            memcpy(&buffer -> buffer[n], op, sizeof(struct operation));
            memcpy(&buffer -> ptrs[n], &i, sizeof(struct operation));
            return;
        }
    }
}


/* Função que lê uma operação do buffer de memória partilhada entre a Main
* e os restaurantes, se houver alguma disponível para ler que seja direcionada ao restaurante especificado.
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma operação disponível, afeta op->id com o valor -1.
*/
void read_main_rest_buffer(struct rnd_access_buffer* buffer, int rest_id, int buffer_size, struct operation* op,struct semaphores* sems){
    int n;
    int val = 0;
    for(n = 0; n < buffer_size; n++){
        if(buffer -> ptrs[n] == 1){
            if(buffer -> buffer[n].requested_rest == rest_id){
                consume_begin(sems->main_rest);
                memcpy(op, &buffer -> buffer[n], sizeof(struct operation));
                buffer -> ptrs[n] = 0;

                val == 1;
                consume_end(sems->main_rest);
                return;

            }
        }
    }
    if(val == 0) op -> id = -1;
    

}


/* Função que lê uma operação do buffer de memória partilhada entre os restaurantes e os motoristas,
* se houver alguma disponível para ler (qualquer motorista pode ler qualquer operação).
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma operação disponível, afeta op->id com o valor -1.
*/
void read_rest_driver_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){

    if(buffer -> ptrs -> in != buffer -> ptrs -> out){
        memcpy(op,&buffer ->buffer[buffer -> ptrs -> out],sizeof(struct operation));
        //buffer -> buffer[buffer -> ptrs -> out].id = -1;
        memset(&buffer -> buffer[buffer -> ptrs -> out].id,-1,sizeof(struct operation));
        buffer -> ptrs -> out = ((buffer -> ptrs ->out) +1) % buffer_size;
    }else{
        op -> id = -1;
    }
}

/* Função que lê uma operação do buffer de memória partilhada entre os motoristas e os clientes,
* se houver alguma disponível para ler dirijida ao cliente especificado. A leitura deve
* ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo. Se não houver
* nenhuma operação disponível, afeta op->id com o valor -1.
*/
void read_driver_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op,struct semaphores* sems){
    int n;
    int val = 0;
    for(n = 0; n < buffer_size; n++){
        if(buffer -> ptrs[n] == 1){
            if(buffer -> buffer[n].requesting_client == client_id){
                consume_begin(sems->driv_cli);
                memcpy(op, &(buffer -> buffer[n]), sizeof(struct operation));
                buffer -> ptrs[n] = 0;
                val == 1;
                consume_end(sems->driv_cli);
                return;
            }
        }
    }
    if (val == 0) op -> id = -1;
}