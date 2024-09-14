#ifndef SCHEDULING_POLICY_H
#define SCHEDULING_POLICY_H

#include "queue.h"  // Include queue.h per definizioni relative alla coda

// Dichiarazioni delle funzioni che implementano le diverse politiche di scheduling
void add_element_to_queue(Queue* q, void* data, int priority);
void* remove_element_from_queue(Queue* q);

#endif // SCHEDULING_POLICY_H
