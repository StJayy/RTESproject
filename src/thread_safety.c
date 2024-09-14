#include "thread_safety.h"

void thread_safety_init(ThreadSafety* ts) {
    pthread_mutex_init(&ts->mutex, NULL);
}

void thread_safety_lock(ThreadSafety* ts) {
    pthread_mutex_lock(&ts->mutex);
}

void thread_safety_unlock(ThreadSafety* ts) {
    pthread_mutex_unlock(&ts->mutex);
}

void thread_safety_destroy(ThreadSafety* ts) {
    pthread_mutex_destroy(&ts->mutex);
}