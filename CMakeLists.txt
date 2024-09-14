cmake_minimum_required(VERSION 3.10)

project(LibraryF VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Aggiungi i file sorgente
set(SOURCES
    src/queue.c
    src/scheduling_policy.c
    src/thread_safety.c
)

# Aggiungi i file di test
set(TEST_SOURCES
    tests/queue_test.c
    tests/queue_test_threads.c
)

# Crea la libreria
add_library(libraryF STATIC ${SOURCES})

# Imposta le directory di inclusione
target_include_directories(libraryF PUBLIC include)

# Crea gli eseguibili di test
add_executable(queue_test tests/queue_test_threads.c)
target_link_libraries(queue_test libraryF)

# Aggiungi il supporto per i thread
find_package(Threads REQUIRED)
target_link_libraries(libraryF Threads::Threads)
target_link_libraries(queue_test Threads::Threads)

# Opzioni di compilazione
target_compile_options(libraryF PRIVATE -Wall -Wextra -pedantic)
target_compile_options(queue_test PRIVATE -Wall -Wextra -pedantic)
