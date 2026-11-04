#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // Para sleep() en segundos

// Declaración de semáforos POSIX
sem_t semAB;
sem_t semBC;

// Hilo A: Desarrollado por Integrante 1
void* HiloA(void* arg) {
    printf("[Hilo A] Paso 1: Iniciando preparación de datos...\n");
    sleep(1); // Simula tiempo de procesamiento (1 segundo)
    printf("[Hilo A] Paso 1: Datos listos. Liberando Hilo B.\n");
    
    // Incrementar semáforo semAB (V / Signal)
    sem_post(&semAB);
    pthread_exit(NULL);
}

// Hilo B: Desarrollado por Integrante 2
void* HiloB(void* arg) {
    // Esperar señal de A (P / Wait)
    sem_wait(&semAB);
    
    printf("[Hilo B] Paso 2: Procesando información de Hilo A...\n");
    sleep(1);
    printf("[Hilo B] Paso 2: Procesamiento terminado. Liberando Hilo C.\n");
    
    // Incrementar semáforo semBC (V / Signal)
    sem_post(&semBC);
    pthread_exit(NULL);
}

// Hilo C: Desarrollado por Integrante 3
void* HiloC(void* arg) {
    // Esperar señal de B (P / Wait)
    sem_wait(&semBC);
    
    printf("[Hilo C] Paso 3: Consolidando y mostrando resultados finales.\n");
    sleep(1);
    printf("[Hilo C] Paso 3: Proceso completo finalizado exitosamente.\n");
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t hA, hB, hC;

    // Inicializar semáforos POSIX
    // sem_init(sem, pshared=0 [local al proceso], valor_inicial=0)
    sem_init(&semAB, 0, 0);
    sem_init(&semBC, 0, 0);

    printf("=== INICIO DEL TALLER DE SINCRONIZACIÓN (POSIX) ===\n\n");

    // Creación de hilos en orden inverso para probar la sincronización
    pthread_create(&hC, NULL, HiloC, NULL);
    pthread_create(&hB, NULL, HiloB, NULL);
    pthread_create(&hA, NULL, HiloA, NULL);

    // Esperar la terminación de los tres hilos
    pthread_join(hA, NULL);
    pthread_join(hB, NULL);
    pthread_join(hC, NULL);

    // Destruir semáforos
    sem_destroy(&semAB);
    sem_destroy(&semBC);

    printf("\n=== FIN DE LA EJECUCIÓN ===\n");
    return 0;
}