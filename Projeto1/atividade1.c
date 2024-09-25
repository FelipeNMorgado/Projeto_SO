#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

// Define o número de threads (2 produtores e 2 consumidor)
#define THREAD_NUM 5

// Declaração dos semáforos para controlar o buffer
sem_t semEmpty;  // Controla os espaços vazios no buffer
sem_t semFull;   // Controla os itens preenchidos no buffer

// Mutex para proteger o acesso ao buffer
pthread_mutex_t mutexBuffer;

// Buffer compartilhado e um contador para rastrear quantos itens estão nele
int buffer[10];
int count = 0;

// Função executada pelos produtores
void* producer(void* args) {
    while (1) {
        // Produz um item (um número aleatório entre 0 e 99)
        int x = rand() % 100;
        sleep(4);  // Simula o tempo necessário para produzir o item

        // Adiciona o item ao buffer
        sem_wait(&semEmpty);  // Espera até que haja espaço disponível no buffer
        pthread_mutex_lock(&mutexBuffer);  // Bloqueia o acesso ao buffer para modificar
        buffer[count] = x;  // Adiciona o item ao buffer
        count++;  // Incrementa o contador de itens no buffer
        pthread_mutex_unlock(&mutexBuffer);  // Libera o acesso ao buffer
        sem_post(&semFull);  // Sinaliza que um item foi adicionado (incrementa semFull)
        printf("Add %d\n" , x);
    }
}

// Função executada pelo consumidor
void* consumer(void* args) {
    while (1) {
        int y;

        // Remove um item do buffer
        sem_wait(&semFull);  // Espera até que haja algum item no buffer
        pthread_mutex_lock(&mutexBuffer);  // Bloqueia o acesso ao buffer para modificar
        y = buffer[count - 1];  // Remove o último item adicionado
        count--;  // Decrementa o contador de itens no buffer
        pthread_mutex_unlock(&mutexBuffer);  // Libera o acesso ao buffer
        sem_post(&semEmpty);  // Sinaliza que há espaço disponível (incrementa semEmpty)

        // Consome o item (imprime o número consumido)
        printf("Got %d\n", y);
        sleep(4);  // Simula o tempo necessário para consumir o item
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));  // Inicializa a semente para geração de números aleatórios
    pthread_t th[THREAD_NUM];  // Array para armazenar os IDs das threads
    pthread_mutex_init(&mutexBuffer, NULL);  // Inicializa o mutex
    sem_init(&semEmpty, 0, 10);  // Inicializa o semáforo semEmpty com valor 10 (buffer vazio)
    sem_init(&semFull, 0, 0);  // Inicializa o semáforo semFull com valor 0 (nenhum item no buffer)

    // Criação das threads (3 consumidor e 2 produtores)
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i < 2) {  // Cria os produtores (threads de índice 0 e 1)
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");  // Erro ao criar thread
            }
        } else {  // Cria o consumidor (thread de índice 2,3)
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");  // Erro ao criar thread
            }
        }
    }

    // Aguarda todas as threads terminarem (o que não acontece pois elas rodam infinitamente)
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");  // Erro ao tentar juntar a thread principal com a thread criada
        }
    }

    // Destrói os semáforos e o mutex ao final (não será alcançado nesse código)
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}
