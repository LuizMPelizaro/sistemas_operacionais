#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int total[NUMBER_OF_RESOURCES];

/*
 * O montante disponivel de cada recurso
 */
int available[NUMBER_OF_RESOURCES];
/*
 *  a demanda máxima de cada cliente
 */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/*
 * O montante correntemente alocado a cada cliente
 */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {0};
/*
 *A necessidade remanescente de cada cliente
 */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int Bavailable[NUMBER_OF_RESOURCES];
int Bmaximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int Ballocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int Bneed[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int Finish[NUMBER_OF_CUSTOMERS] = {0};


int release_resources(int customer_num);

int request_resources(int customer_num, int request[]);

void *thread_func(void *customer_numt);

int banker_algorithm(int customer_num, int request[]);

void print_state();

pthread_mutex_t mutex;
char string[NUMBER_OF_RESOURCES * 2] = {0};
int safeSequence[NUMBER_OF_CUSTOMERS] = {0};


int main(int argc, const char *argv[]) {
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    int e = 4;
    char *returnV;

    for (int i = 0; i < argc - 1; i++) {
        available[i] = atoi(argv[i + 1]);
        total[i] = available[i];
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        for (int j = 0; j < NUMBER_OF_CUSTOMERS; j++) {
            maximum[j][i] = rand() % (total[i] + 1);
            need[j][i] = maximum[j][i];
        }
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        string[i * 2] = i + 'A';
        string[i * 2 + 1] = ' ';
    }
    printf("Recursos totais do sistema:\n");
    printf("%s\n\n", string);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d ", total[i]);
    }
    printf("\nProcessos:\n");
    printf("   %s\n", string);
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("P%d ", i + 1);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }
    print_state();

    pthread_mutex_init(&mutex, NULL);

    pthread_t process1;
    pthread_t process2;
    pthread_t process3;
    pthread_t process4;
    pthread_t process5;

    pthread_create(&process1, NULL, thread_func, &a);
    pthread_create(&process2, NULL, thread_func, &b);
    pthread_create(&process3, NULL, thread_func, &c);
    pthread_create(&process4, NULL, thread_func, &d);
    pthread_create(&process5, NULL, thread_func, &e);

    pthread_join(process1, (void **) &returnV);
    pthread_join(process2, (void **) &returnV);
    pthread_join(process3, (void **) &returnV);
    pthread_join(process4, (void **) &returnV);
    pthread_join(process5, (void **) &returnV); // wait for all the 5 threads to terminate
    return 0;
}

void *thread_func(void *Tcustomer_num) {
    int *c = (int *) Tcustomer_num;
    int customer_num = *c;
    int requestSum = 0;
    while (!Finish[customer_num]) {
        requestSum = 0;
        int request[NUMBER_OF_RESOURCES] = {0};
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            request[i] = rand() % (need[customer_num][i] + 1);
            requestSum = requestSum + request[i];
        }
        if (requestSum != 0)
            while (request_resources(customer_num, request) == -1);
    }
    return 0;
}

int request_resources(int customer_num, int request[]) {
    int returnValue = -1;
    pthread_mutex_lock(&mutex);
    printf("\nP%d solicita ", customer_num + 1);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d ", request[i]);
    }
    printf("\n");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {

        if (request[i] > available[i]) {
            printf("P%d está aguardando os recursos...\n", customer_num + 1);
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }
    returnValue = banker_algorithm(customer_num, request);
    if (returnValue == 0) {
        int needIsZero = 1;
        printf("Uma sequencia segura foi encontrada: ");
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            printf("P%d ", safeSequence[i] + 1);
        }
        printf("\nP%d pedido foi concedido\n", customer_num + 1);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            allocation[customer_num][j] = allocation[customer_num][j] + request[j];
            available[j] = available[j] - request[j];
            need[customer_num][j] = need[customer_num][j] - request[j];
            if (need[customer_num][j] != 0) {
                needIsZero = 0;
            }
        }
        if (needIsZero) {
            Finish[customer_num] = 1;
            release_resources(customer_num);
        }
        print_state();
    } else {
        printf("não conseguiu encontrar uma sequência segura\n");
    }
    pthread_mutex_unlock(&mutex);
    return returnValue;
}

int release_resources(int customer_num) {
    printf("P%d libera todos os recursos\n", customer_num + 1);
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
        available[j] = available[j] + allocation[customer_num][j];
        allocation[customer_num][j] = 0;
    }
    return 0;
}

int banker_algorithm(int customer_num, int request[]) {
    int finish[NUMBER_OF_CUSTOMERS] = {0};
    int count = 0;
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        Bavailable[i] = available[i];
        for (int j = 0; j < NUMBER_OF_CUSTOMERS; j++) {
            Ballocation[j][i] = allocation[j][i];
            Bmaximum[j][i] = maximum[j][i];
            Bneed[j][i] = need[j][i];
        }
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        Bavailable[i] = Bavailable[i] - request[i];
        Ballocation[customer_num][i] = Ballocation[customer_num][i] + request[i];
        Bneed[customer_num][i] = Bneed[customer_num][i] - request[i];
    }
    while (1) {
        int I = -1;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            int nLessThanA = 1;
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                if (Bneed[i][j] > Bavailable[j] || finish[i] == 1) {
                    nLessThanA = 0;
                    break;
                }
            }
            if (nLessThanA) {
                I = i;
                break;
            }
        }
        if (I != -1) {
            safeSequence[count] = I;
            count++;
            finish[I] = 1;
            for (int k = 0; k < NUMBER_OF_RESOURCES; k++) {
                Bavailable[k] = Bavailable[k] + Ballocation[I][k];
            }
        } else {
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                if (finish[i] == 0) {
                    return -1;
                }
            }
            return 0;
        }
    }
}

void print_state() {
    printf("Processos (recursos alocados):\n");
    printf("   %s\n", string);
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("P%d ", i + 1);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }
    printf("Processos (possivelmente recursos necessários ):\n");
    printf("   %s\n", string);
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("P%d ", i + 1);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    printf("Os recursos disponíveis do sistema são:\n");
    printf("%s\n", string);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");
}



