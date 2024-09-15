#ifndef QUEUE_H
#define QUEUE_H
#include "thread_safety.h"
#include <stddef.h>

#ifndef MAX_PRIORITY
#define MAX_PRIORITY 10
#endif

// Definizione delle politiche di scheduling
typedef enum {
    FIFO,
    LIFO,
    PRIORITY
} SchedulingPolicy;

// Struttura per ogni elemento della coda
typedef struct QueueElement {
    void* data;                      // Puntatore ai dati dell'elemento
    int priority;                    // Priorità dell'elemento
    int age;                         // Variabile per aging priority
    struct QueueElement* next;       // Puntatore all'elemento successivo nella coda
} QueueElement;

typedef struct {
    void* data;
    int priority;
    int age;
} ArrayElementData;

// Struttura della coda
typedef struct {
    QueueElement* head;              // Puntatore al primo elemento della coda
    QueueElement* tail;              // Puntatore all'ultimo elemento della coda
    int size;                        // Numero di elementi nella coda
    SchedulingPolicy policy;         // Politica di scheduling della coda
    ThreadSafety ts;                 // Mutex per la sicurezza delle operazioni di base della coda
    pthread_t aging_thread;          // Thread per l'aggiornamento delle priorità
    pthread_cond_t update_cond;      // Condizione per l'aggiornamento delle priorità
    pthread_mutex_t update_mutex;    // Mutex per l'aggiornamento delle priorità
    int should_terminate;            // Flag per indicare se il thread di aggiornamento deve terminare
} Queue;

// Dichiarazione delle funzioni pubbliche della coda
void queue_init(Queue* q, SchedulingPolicy policy);
void queue_destroy(Queue* q);
void queue_push(Queue* q, void* data, int priority);
void* queue_pull(Queue* q);
void* aging_thread_function(void* arg);
void update_priorities(Queue* q);
void print_queue_state(Queue* q);
ArrayElementData* queue_to_array(Queue* q, int* size);

#endif // QUEUE_H
