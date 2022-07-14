//
// Created by luiz- on 10/05/2022.
//

#include <malloc.h>
#include <stdio.h>

#ifndef ATIVIDADE_THREDS_ALLOCATE_PID_H
#define ATIVIDADE_THREDS_ALLOCATE_PID_H

#endif //ATIVIDADE_THREDS_ALLOCATE_PID_H

#define MIN_PID 300
#define MAX_PID 5000

int *array;

/*
 * Cria e inicializa uma estrutura de dados para representar PIDs
 * Retorna: -1, se não for bem-sucedida, e
 *           1, se for bem-sucedida.
 */
int allocate_map(void) {
    array = malloc((MAX_PID - MIN_PID) * sizeof(int));
    int i;
    if (array == NULL) {
        printf("ERRO DE ALOCACAO DE MEMORIA");
        return -1;
    }
    for (i = 0; i < MAX_PID - MIN_PID; i++) {
        *(array + i) = 0;
    }
    return 1;
}

/*
 * Aloca e retorna um PID;
 * Retorna: -1 se for incapaz de alocar um pid (todos os PIDs estão em uso), caso contrário, o valor do ID alocado
 */
int allocate_pid(void) {
    int i;
    for (i = 0; i < MAX_PID - MIN_PID; i++) {
        if (*(array + i) == 0) {
            *(array + i) = 1;
            return (i + MIN_PID);
        }
    }
    return -1;
}

/*
 * Libera um PID
 */

void release_pid(int pid) {
    int i;
    for (i = 0; i < MAX_PID - MIN_PID; i++) {
        if (pid == i + MIN_PID) {
            *(array + i) = 0;
        }
    }
}

