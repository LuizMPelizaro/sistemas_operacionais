#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*
 * Estrutura para passagem de dados aos threds classificadores
 */
struct parameters {
    int *head;
    size_t size_array;
    int tail;
};

void *mergeSortThread(void *pv);

void merge(int *head, int *middle, const int *tail);

void mergeSortMt(int *start, size_t size, int depth);

void mergeSort(int *start, size_t size);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    unsigned int size_array = 10;
    int *data = malloc(size_array * sizeof(*data));
    int test_data[10] = {7,12,19,3,18,4,2,6,15,8};

    for (int i = 0; i < size_array; ++i) {
        data[i] = test_data[i];
        printf("%d ", data[i]);
    }

    printf("\n");

    mergeSort(data, size_array);

    for (int i = 0; i < size_array; ++i) {
        printf("%d ", data[i]);
    }

    printf("\n");

    free(data);

    return 0;
}

void merge(int *start, int *middle, const int *tail) {
    int *res = malloc((tail - start) * sizeof(*res));
    int *lhs = start, *rhs = middle, *dst = res;

    while (lhs != middle && rhs != tail) {
        *dst++ = (*lhs < *rhs) ? *lhs++ : *rhs++;
    }
    while (lhs != middle) {
        *dst++ = *lhs++;
    }

    memcpy(start, res, (rhs - start) * sizeof *res);
    free(res);
}

void mergeSortMt(int *start, size_t size, int depth) {
    if (size < 2)
        return;

    if (depth <= 0 || size < 4) {
        mergeSortMt(start, size / 2, 0);
        mergeSortMt(start + size / 2, size - size / 2, 0);
    } else {
        struct parameters param = {
                start,
                size / 2,
                depth / 2
        };

        pthread_t thread;

        pthread_mutex_lock(&mutex);
        printf("Iniciando a sub thread \n");
        pthread_mutex_unlock(&mutex);

        pthread_create(&thread, NULL, mergeSortThread, &param);

        mergeSortMt(start + size / 2,
                    size - size / 2,
                    depth / 2);

        pthread_join(thread, NULL);

        pthread_mutex_lock(&mutex);
        printf("Encerrando a sub thread \n");
        pthread_mutex_unlock(&mutex);
    }
    merge(start, start + size / 2, start + size);
}

void *mergeSortThread(void *pv) {
    struct parameters *param = pv;
    mergeSortMt(param->head, param->size_array, param->tail);
    return pv;
}

void mergeSort(int *start, size_t size) {
    mergeSortMt(start, size, 4);
}
