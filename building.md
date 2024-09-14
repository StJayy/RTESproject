# Istruzioni di Compilazione per LibraryF

## Requisiti

- CMake (versione 3.10 o superiore)
- Compilatore C compatibile con C11
- pthread library

## Passi per la Compilazione

1. Se non è già presente, crea una directory di build:

mkdir build cd build


2. Configura il progetto con CMake:

cmake ..


## Compilazione su Sistemi Specifici

### Windows con MinGW

1. Assicurati che MinGW sia installato e nel PATH di sistema.
2. Usa il seguente comando per configurare:

cmake -G "MinGW Makefiles" ..

3. Compila con:

mingw32-make


### Linux

1. Installa le dipendenze necessarie (cmake, gcc, libpthread-stubs0-dev).
2. Segui i passi standard di compilazione.

### macOS

1. Installa Xcode Command Line Tools e CMake.
2. Segui i passi standard di compilazione.

## Risoluzione dei Problemi

- Se incontri errori relativi a pthread, assicurati che la libreria sia installata nel tuo sistema.
- Per problemi specifici del compilatore, verifica di utilizzare una versione compatibile con C11.

## Esecuzione dei Test

Dopo la compilazione, puoi eseguire i test con:

./queue_test_threads


Per ulteriori informazioni o assistenza, consulta la documentazione di CMake o apri una issue nel repository del progetto.
