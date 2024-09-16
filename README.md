# LibraryF - Implementazione di Coda Thread-Safe

LibraryF è una libreria C che fornisce un'implementazione di coda thread-safe con supporto per diverse politiche di scheduling.

## Caratteristiche

- Operazioni di coda thread-safe
- Supporto per politiche di scheduling FIFO, LIFO e a Priorità
- Meccanismo di invecchiamento delle priorità per lo scheduling a Priorità
- Suite di test completa

## File

- `CMakeLists.txt`: File di configurazione CMake
- `include/libraryF.h`: Header principale della libreria
- `include/queue.h`: Interfaccia della coda
- `include/scheduling_policy.h`: Definizioni delle politiche di scheduling
- `include/thread_safety.h`: Utilità per la sicurezza dei thread
- `src/queue.c`: Implementazione della coda
- `src/scheduling_policy.c`: Implementazione delle politiche di scheduling
- `src/thread_safety.c`: Implementazione della sicurezza dei thread
- `tests/queue_test_threads.c`: Test multi-thread della coda

## Compilazione del Progetto

Per le istruzioni dettagliate sulla compilazione del progetto, si prega di consultare il file [building.md](building.md).


## Esecuzione dei Test

Dopo la compilazione, puoi eseguire il test della coda:

```bash
./queue_test
```

## Utilizzo

Per utilizzare la libreria nel tuo progetto, includi l'header principale:

```c
#include "libraryF.h"
```

## Conclusione
Questo README fornisce una panoramica del tuo progetto, elenca i file principali, spiega come compilare e testare la libreria e fornisce un breve esempio di utilizzo. Puoi personalizzarlo ulteriormente aggiungendo istruzioni d'uso più dettagliate, documentazione API o qualsiasi altra informazione rilevante sul tuo progetto.

