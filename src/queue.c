#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
#define HAS_CLOCK_REALTIME
#endif

#include "queue.h"
#include "scheduling_policy.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void queue_init(Queue* q, SchedulingPolicy policy) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    q->policy = policy;
    thread_safety_init(&q->ts); //Mutex che protegge le operazioni di base della coda (push, pull) 
    if (policy == PRIORITY){
        pthread_create(&q->aging_thread, NULL, aging_thread_function, q);
    }

    //Mutex + condition che gestiscono specificatamente l'aggiornamento delle priorità
    pthread_mutex_init(&q->update_mutex, NULL);
    pthread_cond_init(&q->update_cond, NULL);   
}

void queue_destroy(Queue* q) {
    QueueElement* current = q->head;

    if (q->policy == PRIORITY){
        pthread_cancel(q->aging_thread);
        pthread_join(q->aging_thread, NULL);
    }

    while (current != NULL) {
        QueueElement* next = current->next;
        free(current);
        current = next;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    thread_safety_destroy(&q->ts);
}


void queue_push(Queue* q, void* data, int priority) {
    thread_safety_lock(&q->ts);
    add_element_to_queue(q, data, priority);
    thread_safety_unlock(&q->ts);
}

void* queue_pull(Queue* q) {
    thread_safety_lock(&q->ts);
    void* result = remove_element_from_queue(q);
    thread_safety_unlock(&q->ts);
    return result;
}

/*
void* aging_thread_function(void* arg) {
    Queue* q = (Queue*)arg;
    struct timespec ts;
    while (1) {
        pthread_mutex_lock(&q->update_mutex);
        //Questa struttura a differenza di un semplice sleep() facilita la coordinazione con altri thread e ne migliora la flessibilità 
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_nsec += 10000000;  //100ms
        pthread_cond_timedwait(&q->update_cond, &q->update_mutex, &ts); //thread rimane bloccato per tempo ts
        pthread_mutex_unlock(&q->update_mutex);

        update_priorities(q);
    }
    return NULL;
}
*/


void* aging_thread_function(void* arg) {
    Queue* q = (Queue*)arg;
    struct timespec ts = {0, 10000000}; // 100ms

    while (1) {
        nanosleep(&ts, NULL);
        update_priorities(q);
    }
    return NULL;
}


void update_priorities(Queue* q) {
    pthread_mutex_lock(&q->update_mutex);
    thread_safety_lock(&q->ts);

    QueueElement* current = q->head;
    while (current != NULL) {
        current->age++;
        if (current->age >= 10 && current->priority < MAX_PRIORITY) {
            current->priority++;
            current->age = 0;
            printf("PROCESSO %d UPGRADE DI PRIORITA A %d\n", *(int*)current->data, current->priority);
        }

        current = current->next;
    }

    thread_safety_unlock(&q->ts);
    pthread_cond_broadcast(&q->update_cond);
    pthread_mutex_unlock(&q->update_mutex);
}


void print_queue_state(Queue* q) {
    thread_safety_lock(&q->ts);
    QueueElement* current = q->head;
    printf("Queue state (%d elements): ", q->size);
    while (current != NULL) {
        printf("(%d, p:%d, a:%d) ", *(int*)current->data, current->priority, current->age);
        current = current->next;
    }
    printf("\n");
    thread_safety_unlock(&q->ts);
}

ArrayElementData* queue_to_array(Queue* q, int* size) {
    thread_safety_lock(&q->ts);
    *size = q->size;
    ArrayElementData* array = malloc(sizeof(ArrayElementData) * q->size);
    if (!array) return NULL;

    QueueElement* current = q->head;
    for (int i = 0; i < q->size; i++) {
        array[i].data = current->data;
        array[i].priority = current->priority;
        array[i].age = current->age;
        current = current->next;
    }
    thread_safety_unlock(&q->ts);
    return array;
}

