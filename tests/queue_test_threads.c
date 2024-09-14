#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include "queue.h"
#include <stdatomic.h>
#include <time.h>


#define NUM_THREADS 5
#define OPERATIONS_PER_THREAD 7

atomic_uint seed;
Queue queue;

//È un generatore di numeri casuali in thread safe, un normale rand()
//puo' dare problemi quando il numero di thread è alto
int generate_random_priority() {
    unsigned int old_seed = atomic_fetch_add(&seed, 1);
    //I due numeri sono costanti scelte specificicatamente per le loro proprietà
    //matematiche ed aiutano a garantire una buona distribuzione dei numeri generati
    return (old_seed * 1103515245 + 12345) % 10 + 1;
}

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    for (int i = 0; i < OPERATIONS_PER_THREAD; i++) {
        int* data = malloc(sizeof(int));
        *data = thread_id * OPERATIONS_PER_THREAD + i;

        //Generatore di numeri cassuali thread_safe
        int priority = generate_random_priority();
        queue_push(&queue, data, priority);

        ArrayElementData* array = queue_to_array(&queue, &queue.size);
        if (array != NULL) {
            for (int j = 0; j < queue.size; j++) {
                if (*(int*)array[j].data == *data) {
                    printf("Thread %d: pushed element %d - Priority: %d, Age: %d\n", 
                            thread_id, *data, priority, array[j].age);
                    break;
                }
            }
            free(array);
        }

        //sleep(1);
        print_queue_state(&queue);

        if (i % 2 == 0) {  // Ogni due push, facciamo un pull
            ArrayElementData* array = queue_to_array(&queue, &queue.size);
            if (array != NULL && queue.size > 0) {
                int* pulled_data = (int*)queue_pull(&queue);
                if (pulled_data != NULL) {
                    printf("thread %d: pulled %d (age: %d)\n", thread_id, *pulled_data, array[0].age);
                    free(pulled_data);
                }
                free(array);
            }
            print_queue_state(&queue);
        }
    }
    return NULL;
}

int main() {
    atomic_store(&seed, (unsigned int)time(NULL));

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Inizializzazione della coda in base alla policy scelta
    printf("Seleziona la scheduling policy (0 per FIFO, 1 per LIFO, 2 per PRIORITY):\n");
    int scheduling_policy;
    scanf("%d", &scheduling_policy);

    if (scheduling_policy == 0) {
        queue_init(&queue, FIFO);
    } else if(scheduling_policy == 1) {
        queue_init(&queue, LIFO);
    } else if(scheduling_policy == 2) {
        queue_init(&queue, PRIORITY);
    } else {
        printf("Policy non valida.\n");
        return 1;
    }

    // Creazione dei thread
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Attesa della terminazione dei thread
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Verifica finale
    printf("Dimensione finale della coda: %d\n", queue.size);
    assert(queue.size >= NUM_THREADS * OPERATIONS_PER_THREAD / 3 && 
           queue.size <= NUM_THREADS * OPERATIONS_PER_THREAD * 2 / 3);
    // Pulizia della coda
    while (queue.size > 0) {
        int* data = queue_pull(&queue);
        free(data);
    }

    queue_destroy(&queue);

    printf("Test concorrenza completato con successo!\n");

    return 0;
}
