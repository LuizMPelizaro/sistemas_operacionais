#include <pthread.h>
#include <stdio.h>

void *runner(void *param);

int main(int argc, char *argv[]){
	pthread_t tid;
	pthread_create(&tid,NULL,runner,argv[1]);
	pthread_join(tid,NULL);
}

void *runner(void *param){
	int i, num = atoi(param);
	int totalDivisores, j;
	printf("1 ");
	for(i = 0; i<= num;i++){
		totalDivisores = 0;
        // Conta os divisores para verificar se o valor é primo
        for(j = 1; j <= i; j++) {
            if(i % j == 0) {
                totalDivisores++;
            }
        }
        if(totalDivisores == 2) {
            printf("%d ", i);
        }
	}
	sleep(1);
	return NULL;
}

