#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Define o número de threads (2 produtores e 2 consumidor)
#define THREAD_NUM 5


// Mutex para proteger o acesso ao buffer
pthread_mutex_t mutexBuffer;

// Criando a variavel de condicao 
pthread_cond_t contentBuffer;

// Buffer compartilhado e um contador para rastrear quantos itens estão nele
int buffer[10];
int count = 0;

// Função executada pelos produtores
void* producer(void* args) {
    while (count<11) {
        // Produz um item (um número aleatório entre 0 e 99)
        int x = rand() % 100;
        sleep(3);  // Simula o tempo necessário para produzir o item

        // Adiciona o item ao buffer
        pthread_mutex_lock(&mutexBuffer);  // Bloqueia o acesso ao buffer para modificar
        buffer[count] = x;  // Adiciona o item ao buffer
        count++;  // Incrementa o contador de itens no buffer
        pthread_mutex_unlock(&mutexBuffer);  // Libera o acesso ao buffer
        pthread_cond_broadcast(&contentBuffer); // Transmite o sinal para todos os consumidores
        printf("Add %d\n" , x);
    }
}

// Função executada pelo consumidor
void* consumer(void* args) {
    int y;
    pthread_mutex_lock(&mutexBuffer);  // Bloqueia o acesso ao buffer para modificar
    // Enquanto o buffer estiver vazio, desbloqueia o mutex e espera por sinal do produtor
    while (count < 1) { 
        pthread_cond_wait(&contentBuffer, &mutexBuffer); 
    }
    y = buffer[count - 1];  // Remove o último item adicionado
    count--;  // Decrementa o contador de itens no buffer
    pthread_mutex_unlock(&mutexBuffer);  // Libera o acesso ao buffer
    

    // Consome o item (imprime o número consumido)
    printf("Got %d\n", y);
    sleep(3);  // Simula o tempo necessário para consumir o item
}

int main(int argc, char* argv[]) {
    srand(time(NULL));  // Inicializa a semente para geração de números aleatórios
    pthread_t th[THREAD_NUM];  // Array para armazenar os IDs das threads
    pthread_mutex_init(&mutexBuffer, NULL);  // Inicializa o mutex
    pthread_cond_init(&contentBuffer, NULL); // Inicializa a variável de condicao

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

    // Destroi o mutex e a variável de condicao
    pthread_mutex_destroy(&mutexBuffer);
    pthread_cond_destroy(&contentBuffer);

    return 0;
}
