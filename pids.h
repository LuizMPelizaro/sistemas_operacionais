//
// Created by luiz- on 10/05/2022.
//

#ifndef ATIVIDADE_THREDS_PIDS_H
#define ATIVIDADE_THREDS_PIDS_H

#endif //ATIVIDADE_THREDS_PIDS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MIN_PID 300
#define MAX_PID 5000


bool *pid_map;

int get_random() {
    return rand() % (MAX_PID - MIN_PID + 1) + MIN_PID;
}
/*
 * Cria e inicializa uma estrutura de dados para representar PIDs
 * Retorna: -1, se não for bem-sucedida, e
 *           1, se for bem-sucedida.
 */
int allocate_map(void) {
    srand(time(0));
    pid_map = malloc(sizeof(bool) * MAX_PID); // yah, allocated extra 300 pid
    return pid_map == NULL ? -1 : 1;
}
/*
 * Aloca e retorna um PID;
 * Retorna: -1 se for incapaz de alocar um pid (todos os PIDs estão em uso), caso contrário, o valor do ID alocado
 */
int allocate_pid() {
    int pid = get_random();
    while (pid_map[pid] == true) {
        pid = get_random();
    }
    pid_map[pid] = true;
    return pid;
}
/*
 * Libera um PID
 */
void release_pid(int pid) {
    if (pid_map[pid] == true) {
        pid_map[pid] = false;
        printf("Release pid %d\n", pid);
    } else {
        printf("PID %d is not associated with any process\n", pid);
    }
}

