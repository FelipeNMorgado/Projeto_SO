#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <errno.h>

// Define o número de threads (2 escritores e 3 leitores)
#define NUM_WRITERS 2
#define NUM_READERS 3
#define THREAD_NUM (NUM_WRITERS + NUM_READERS)

// Buffer compartilhado
int shared_data = 0;

// Contadores para controlar a prioridade dos escritores
int readers_count = 0;
int writers_waiting = 0;
int writers_active = 0;

// Variáveis de sincronização
pthread_rwlock_t rwlock;
pthread_mutex_t mutex_priority;

// Função executada pelos escritores (produtores)
void* writer(void* args) {
    while (1) {
        // Escritores têm prioridade, por isso usam rwlock para bloquear leitores e outros escritores
        pthread_mutex_lock(&mutex_priority);
        writers_waiting++;
        pthread_mutex_unlock(&mutex_priority);

        pthread_rwlock_wrlock(&rwlock);  // Escritor precisa de acesso exclusivo
        pthread_mutex_lock(&mutex_priority);
        writers_waiting--;
        writers_active++;
        pthread_mutex_unlock(&mutex_priority);

        // Escrita no buffer compartilhado
        shared_data = rand() % 100;
        printf("Escritor escreveu: %d\n", shared_data);
        sleep(1);  // Simula o tempo necessário para escrever

        pthread_rwlock_unlock(&rwlock);  // Libera o recurso para outros
        pthread_mutex_lock(&mutex_priority);
        writers_active--;
        pthread_mutex_unlock(&mutex_priority);

        sleep(2);  // Aguarda antes de tentar escrever novamente
    }
}

// Função executada pelos leitores (consumidores)
void* reader(void* args) {
    while (1) {
        pthread_mutex_lock(&mutex_priority);
        // Leitores só podem acessar se não houver escritores ativos ou esperando
        while (writers_waiting > 0 || writers_active > 0) {
            pthread_mutex_unlock(&mutex_priority);
            sched_yield();  // Cede o processador para dar prioridade aos escritores
            pthread_mutex_lock(&mutex_priority);
        }
        readers_count++;
        pthread_mutex_unlock(&mutex_priority);

        pthread_rwlock_rdlock(&rwlock);  // Leitor obtém acesso compartilhado
        printf("Leitor leu: %d\n", shared_data);
        pthread_rwlock_unlock(&rwlock);  // Libera o recurso

        pthread_mutex_lock(&mutex_priority);
        readers_count--;
        pthread_mutex_unlock(&mutex_priority);

        sleep(2);  // Simula o tempo de leitura
    }
}

int main() {
    srand(time(NULL));

    // Inicializa o rwlock e mutex
    pthread_rwlock_init(&rwlock, NULL);
    pthread_mutex_init(&mutex_priority, NULL);

    // Criação das threads
    pthread_t threads[THREAD_NUM];

    // Criar escritores
    for (int i = 0; i < NUM_WRITERS; i++) {
        if (pthread_create(&threads[i], NULL, writer, NULL) != 0) {
            perror("Erro ao criar thread de escritor");
        }
    }

    // Criar leitores
    for (int i = NUM_WRITERS; i < THREAD_NUM; i++) {
        if (pthread_create(&threads[i], NULL, reader, NULL) != 0) {
            perror("Erro ao criar thread de leitor");
        }
    }

    // Aguarda todas as threads terminarem (o que não acontece pois elas rodam infinitamente)
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destrói o rwlock e o mutex
    pthread_rwlock_destroy(&rwlock);
    pthread_mutex_destroy(&mutex_priority);

    return 0;
}
