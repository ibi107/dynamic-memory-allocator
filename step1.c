#include "functions.c"

void initializeMemory(int memSize, struct space *mem) {
    mem->memory = malloc(memSize * sizeof(char));
    mem->sizes = malloc(memSize * sizeof(int));
    mem->len = memSize;

    for (int i = 0; i < mem->len; i++) {
        mem->memory[i] = FREE;
        mem->sizes[i] = FREESIZE;
    }

    printMemory(mem);
    printSizes(mem);
}

void cleanMemory(struct space *mem) {
    for (int i = 0; i < mem->len; i++) {
        mem->memory[i] = FREE;
        mem->sizes[i] = FREESIZE;
    }
    
    printMemory(mem);
    printSizes(mem);
    
    free(mem->memory);
    free(mem->sizes);
}