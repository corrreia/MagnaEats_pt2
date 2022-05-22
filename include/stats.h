#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "main.h"

/**
 * Função que cria um novo ficheiro de stats com o nome do fileName
 * 
 * @param fileName - nome do ficheiro a ser criado
 */
void createStatsFile(char * fileName);


/**
 * Função que escreve as estatísticas finais do magnaeats tendo em conta
 * o argumento data.
 * 
 * @param data - estrutura que contém todas as informações a escrever no ficheiro
 */
void doStats(struct main_data* data);

/**
 * Função que fecha um ficheiro de estatisticas
 * 
 */
void closeStatsFile();

#endif