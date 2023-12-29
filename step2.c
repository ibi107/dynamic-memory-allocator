#include <functions.c>

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

int stackAllocator(int nbytes, struct space *mem) {
    int t0 = 0;
    while (t0 < mem->len && mem->sizes[t0] != FREESIZE) {
        t0++;
    }

    if (t0 + nbytes > mem->len) {
        return mem->len;
    }
    
    mem->memory[t0] = BUSY;
    mem->sizes[t0] = nbytes;
    for (int t = 1; t < nbytes; t++) {
        mem->memory[t0 + t] = BUSY;
        mem->sizes[t0 + t] = BUSYSIZE;
    }

    return t0;
}

void deallocator(int p, struct space *mem) {
    if (p == mem->len || p < 0) {
        return;
    }

    int nbytes = mem->sizes[p];
    for (int t = 0; t < nbytes; t++) {
        mem->memory[p + t] = FREE;
        mem->sizes[p + t] = FREESIZE;
    }
}