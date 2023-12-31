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
    // Allocate memory blocks and initialise them to FREE/FREESIZE
    mem->memory = malloc(memSize * sizeof(char));
    mem->sizes = malloc(memSize * sizeof(int));
    mem->len = memSize;
    for (int i = 0; i < mem->len; i++) {
        mem->memory[i] = FREE;
        mem->sizes[i] = FREESIZE;
    }
    
    // Print memory blocks
    printMemory(mem);
    printSizes(mem);
}

void cleanMemory(struct space *mem) {
    // Clean memory by setting blocks back to FREE/FREESIZE
    for (int i = 0; i < mem->len; i++) {
        mem->memory[i] = FREE;
        mem->sizes[i] = FREESIZE;
    }
    
    // Print and free memory blocks
    printMemory(mem);
    printSizes(mem);
    free(mem->memory);
    free(mem->sizes);
}

/* Step 2 */

int stackAllocator(int nbytes, struct space *mem) {
    // Search for first entry marked with FREESIZE
    int i = 0;
    while (i + nbytes < mem->len && mem->sizes[i] != FREESIZE) {
        i++;
    }
    
    // Return mem->len if no space for nbytes
    if (i + nbytes >= mem->len) {
        return mem->len;
    }
    
    // Allocate nbytes with BUSY/BUSYSIZE
    int j = 0;
    while (j < nbytes && i + j < mem->len) {
        mem->memory[i + j] = BUSY;
        mem->sizes[i + j] = BUSYSIZE;
        j++;
    }
    mem->sizes[i] = nbytes;
    
    // Return index of first entry for nbytes
    return i;
}

void deallocator(int p, struct space *mem) {
    // Return if index is invalid
    if (p == mem->len || p < 0) {
        return;
    }
    
    // Deallocate memory in use for block at p
    int nbytes = mem->sizes[p];
    int i = 0;
    while (i < nbytes) {
        mem->memory[p + i] = FREE;
        mem->sizes[p + i] = FREESIZE;
        i++;
    }
}

/* Step 3 */

int spaceScanner(int nbytes, struct space *mem) {
    // Find nbytes of consecutive FREESIZE memory in sizes
    int i = 0;
    int j = 0;
    while (j == 0 && i < mem->len) {
        int k = 0;
        while (i + k < mem->len && mem->sizes[i + k] == FREESIZE) {
            k++;
        }
        if (k > nbytes) {
            j = 1;
        } else {
            i++;
        }
    }

    // Return starting index of nbytes free block (mem->len if none exist)
    return i;
}

int heapAllocatorQ3(int nbytes, struct space *mem) {
    // If no free nbytes block return mem->len
    int i = spaceScanner(nbytes, mem);
    if (i == mem->len) {
        return i;
    }
    
    // Allocate memory in nbytes block
    int j = 0;
    while (j < nbytes) {
        mem->memory[i + j] = BUSY;
        mem->sizes[i + j] = BUSYSIZE;
        j++;
    }
    mem->sizes[i] = nbytes;
    
    // Return starting index of allocated block
    return i;
}

/* Step 4 */

void copyArray(int *aIn, int *aOut, int len) {
    // Copy value of each aIn index to each aOut index
    int i = 0;
    while (i < len) {
        aOut[i] = aIn[i];
        i++;
    }
}

void increaseMemory(int nbytes, struct space *mem) {
    // Compute length of new memory
    int newLen = mem->len;
    while (newLen - mem->len < nbytes) {
        newLen = 2 * (newLen + 1);
    }
    
    // Copies the content of old memory to new memory
    char *s = mem->memory;
    int *a = mem->sizes;
    int l = mem->len;
    initializeMemory(newLen, mem);
    copyString(s, mem->memory, l);
    copyArray(a, mem->sizes, l);
    
    // Free old memory and sizes blocks
    free(s);
    free(a);
}

int heapAllocator(int nbytes, struct space *mem) {
    // Increase memory until a suitable free nbytes block is found
    int i = spaceScanner(nbytes, mem);
    while (i == mem->len) {
        increaseMemory(nbytes, mem);
        i = spaceScanner(nbytes, mem);
    }
    
    // Allocate the memory block with BUSY/BUSYSIZE
    int j = 0;
    while (j < nbytes) {
        mem->memory[i + j] = BUSY;
        mem->sizes[i + j] = BUSYSIZE;
        j++;
    }
    mem->sizes[i] = nbytes;
    
    // Return starting index of allocated block
    return i;
}

int readString(char **s) {
    // Dynamically construct a string from standard input
    int i = 0;
    char c = getchar();
    *s = malloc(1);
    **s = '\0';
    while (c != '\n' && c != EOF) {
        char *p = *s;
        *s = malloc(i + 2); // Allocate memory to accomodate new character and null character

        // Copy old string to new string, assign new character and null character to *s
        copyString(p, *s, i);
        (*s)[i] = c;
        (*s)[i + 1] = '\0';
        free(p); // Free old string

        i++;
        c = getchar(); 
    }
    
    if (c == EOF) {
        return 0;
    }
    return 1;
}