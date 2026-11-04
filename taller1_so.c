#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Recurso compartido
int variable_compartida = 0;

// Mutex para Exclusión Mutua en la variable compartida
pthread_mutex_t mutex_variable;

// Semáforos POSIX para Sincronización de secuencia
sem_t semA_C; // Notifica a C que A escribió Cifra 1
sem_t semC_B; // Notifica a B que C leyó y limpió la variable
sem_t semB_C; // Notifica a C que B escribió Cifra 2

// --- INTEGRANTE 1: Hilo A ---
void* HiloA(void* arg) {
    int cifra_a = 15;
    
    pthread_mutex_lock(&mutex_variable);
    variable_compartida = cifra_a;
    printf("[Hilo A] Escribió la cifra: %d\n", variable_compartida);
    pthread_mutex_unlock(&mutex_variable);
    
    sem_post(&semA_C);
    return NULL;
}

// --- INTEGRANTE 2: Hilo B ---
void* HiloB(void* arg) {
    int cifra_b = 25;
    
    sem_wait(&semC_B);
    pthread_mutex_lock(&mutex_variable);
    variable_compartida = cifra_b;
    printf("[Hilo B] Escribió la cifra: %d\n", variable_compartida);
    pthread_mutex_unlock(&mutex_variable);

    sem_post(&semB_C);
    return NULL;
}

// --- INTEGRANTE 3: Hilo C ---
void* HiloC(void* arg) {
    int cifra1 = 0, cifra2 = 0;

    // --- PARTE 1: Procesar Cifra 1 ---
    sem_wait(&semA_C);
    pthread_mutex_lock(&mutex_variable);
    cifra1 = variable_compartida;
    printf("[Hilo C] Leyó Cifra 1: %d\n", cifra1);
    variable_compartida = 0;
    printf("[Hilo C] Limpió variable_compartida = 0\n");
    pthread_mutex_unlock(&mutex_variable);

    // Dar paso a Hilo B
    sem_post(&semC_B);

    // --- PARTE 2: Procesar Cifra 2 y Consolidar ---
    sem_wait(&semB_C);
    pthread_mutex_lock(&mutex_variable);
    cifra2 = variable_compartida;
    printf("[Hilo C] Leyó Cifra 2: %d\n", cifra2);
    pthread_mutex_unlock(&mutex_variable);

    // Renderizado final
    printf("\n================ RENDERIZADO DE RESULTADOS ================\n");
    printf("-> Cifra enviada por Hilo A: %d\n", cifra1);
    printf("-> Cifra enviada por Hilo B: %d\n", cifra2);
    printf("-> SUMA TOTAL CALCULADA POR HILO C: %d\n", cifra1 + cifra2);
    printf("===========================================================\n");

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t hA, hB, hC;

    // Inicialización de Mutex y Semáforos
    pthread_mutex_init(&mutex_variable, NULL);
    sem_init(&semA_C, 0, 0);
    sem_init(&semC_B, 0, 0);
    sem_init(&semB_C, 0, 0);

    printf("=== INICIO DE LA PRUEBA DE FUEGO ===\n\n");

    // Creación de hilos (en orden inverso para demostrar sincronización estricta)
    pthread_create(&hC, NULL, HiloC, NULL);
    pthread_create(&hB, NULL, HiloB, NULL);
    pthread_create(&hA, NULL, HiloA, NULL);

    // Esperar a que terminen
    pthread_join(hA, NULL);
    pthread_join(hB, NULL);
    pthread_join(hC, NULL);

    // Destrucción de recursos
    pthread_mutex_destroy(&mutex_variable);
    sem_destroy(&semA_C);
    sem_destroy(&semC_B);
    sem_destroy(&semB_C);

    printf("\n=== FIN DE LA EJECUCIÓN ===\n");
    return 0;
}
