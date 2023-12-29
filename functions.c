#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUSY '+'
#define FREE '_'
#define BUSYSIZE -1
#define FREESIZE 0

struct space {
    char *memory;
    int *sizes;
    int len;
};

/* Auxiliary functions */

void printMemory(struct space *mem) {
    int i = 0;
    while (i < mem->len) {
		printf("%c", *(mem->memory + i));
        i = i + 1;
    }
	printf("\n");
}

void printSizes(struct space *mem) {
    int i = 0;
    int c;
	while (i < mem->len) {
        int n = *(mem->sizes + i);
        int t = 10000;
        while (n > 9) {
            c = n/t;
            n = n - c * t;
            t = t / 10;
            if (c) {
                c =  c % 10 + '0';
				printf("%c", c);
                i = i + 1;
            }
        }   
		c =  n % 10 + '0';
		printf("%c", c);
        i = i + 1;
    }
	printf("\n");
}

void copyString(char *sIn, char *sOut, int len) {
    int t = 0;
    while (t < len) {
        *(sOut + t) = *(sIn + t);
        t = t + 1;
    }
}

int stringLen(char *s) {
    int t = 0;
    while (*(s + t) != '\0')
        t++;
    return t;
}

/* Step 1 */

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

/* Step 2 */

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