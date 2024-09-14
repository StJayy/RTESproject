#ifndef THREAD_SAFETY_H
#define THREAD_SAFETY_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
} ThreadSafety;

void thread_safety_init(ThreadSafety* ts);
void thread_safety_lock(ThreadSafety* ts);
void thread_safety_unlock(ThreadSafety* ts);
void thread_safety_destroy(ThreadSafety* ts);

#endif // THREAD_SAFETY_H
