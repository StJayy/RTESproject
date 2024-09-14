#include "scheduling_policy.h"
#include <stdlib.h> // Necessario per malloc e free

void add_element_to_queue(Queue* q, void* data, int priority) {
    QueueElement* newElement = (QueueElement*)malloc(sizeof(QueueElement));
    if (newElement == NULL) {
        return; // Gestione dell'errore di allocazione
    }
    newElement->data = data;
    newElement->priority = (priority > MAX_PRIORITY) ? MAX_PRIORITY : priority; //Controllo priorità, se viene fornito un valore di priorità maggiore di MAX_PRIORITY, lo impostiamo a MAX_PRIORITY
    newElement->next = NULL;
    newElement->age = 0;

    if (q->policy == FIFO) {
        // Per FIFO, aggiungiamo sempre alla fine della coda
        if (q->tail != NULL) {
            q->tail->next = newElement; 
        }
        q->tail = newElement; //Aggiorno il puntatore tail della coda affinché punti al nuovo elemento
        if (q->head == NULL) {
            q->head = newElement; //Se la coda era vuota, questo elemento è anche il head
        }
    } else if (q->policy == LIFO) {
        // Per LIFO, aggiungiamo sempre all'inizio della coda
        newElement->next = q->head;
        q->head = newElement;
        if (q->tail == NULL) {
            q->tail = newElement;  // Se la coda era vuota, questo elemento è anche il tail
        }
    } else if (q->policy == PRIORITY) {
        // Per la coda con priorità, inseriamo in base alla priorità, il valore priorità più basso va messo all'inizio della coda
        if (q->head == NULL || q->head->priority > priority) {
            newElement->next = q->head;
            q->head = newElement;
            if (q->tail == NULL) {
                q->tail = newElement;
            }
        } else {
            QueueElement* current = q->head;
            // Cerco dove inserire il nuovo elemento 
            while (current->next != NULL && current->next->priority <= priority) {
                current = current->next;
            }
            newElement->next = current->next;
            current->next = newElement;
            if (current->next == NULL) {
                q->tail = newElement; // Aggiornamento del tail se l'elemento è aggiunto alla fine
            }
        }
    }
    q->size++;
}

void* remove_element_from_queue(Queue* q) {
    if (q->head == NULL) {
        return NULL; // Coda vuota
    }

    QueueElement* temp = q->head;
    void* data = temp->data;

    if (q->policy == FIFO || q->policy == LIFO) {
        // Per FIFO e LIFO, rimuoviamo sempre il primo elemento
        q->head = q->head->next;
        if (q->head == NULL) {
            q->tail = NULL; // Se la coda si svuota
        }
    } else if (q->policy == PRIORITY) {
        // Per la priorità con aging, troviamo l'elemento con la priorità effettiva più alta
        QueueElement* current = q->head;
        QueueElement* prev = NULL;
        QueueElement* highest_priority = current;
        QueueElement* highest_priority_prev = NULL;

        while (current != NULL) {
            //int current_priority = current->priority + (current->age / 10);
            int current_priority = current->priority;
            int  highest_priority_value = highest_priority->priority;
            //int highest_priority_value = highest_priority->priority + (highest_priority->age / 10);

            if (current_priority > highest_priority_value) {
                highest_priority = current;
                highest_priority_prev = prev;
            }

            prev = current;
            current = current->next;
        }

        // Rimuoviamo l'elemento con la priorità più alta
        if (highest_priority_prev == NULL) {
            q->head = highest_priority->next;
        } else {
            highest_priority_prev->next = highest_priority->next;
        }

        if (highest_priority == q->tail) {
            q->tail = highest_priority_prev;
        }

        temp = highest_priority;
        data = temp->data;
    }


    free(temp);
    q->size--;
    return data;
}
