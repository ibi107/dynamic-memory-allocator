# Dynamic Memory Allocator
## Introduction
This project is a simulation for a dynamical memory allocator. The `memory` heap is represented as an array of characters in different states e.g. `_` represents `FREE` and `+` represents `BUSY`. Alongside the heap, a `sizes` array (of same length) is also maintained which signifies which `memory` bytes are `BUSY`. For instance:
```
HELLO++___
7//////000
```
- *Note that `memory` replaces `BUSY` bytes with the string allocated, and `sizes` replaces busy bytes with the length of the string*.
- *Also note that `memory` allocates two extra bytes (`++`) after the string for visual purposes*.
  
The program also simulates fragmentation by randomly deallocating blocks of memory (see [Flow](#flow)).

## Flow
The program starts with `10` bytes of memory allocated in the heap:
```
__________
0000000000
```
Say we want to allocate the string "DYNAMIC MEMORY ALLOCATOR". We firstly allocate "DYNAMIC":
```
DYNAMIC++_
9////////0
```
Now, we want to allocate another string, but there isn't enough space in the heap. So, memory is automatically increased in the heap with `increaseMemory()`:
```
DYNAMIC++_____________
9////////0000000000000
```
- *Note that the memory is increased by `len = 2 * (len + 1)`*
  
Now we can allocate another string, "MEMORY":
```
DYNAMIC++MEMORY++_____
9////////8///////00000
```
We now allocate "ALLOCATOR":
```
DYNAMIC++________ALLOCATOR++__________________
9////////0000000011/////////000000000000000000
```
Oh no! it seems "MEMORY" was deallocated due to **fragmentation** within our system. Let's reallocate "MEMORY":
```
DYNAMIC++MEMORY++ALLOCATOR++__________________
9////////8///////11/////////000000000000000000
```
- *Note that "MEMORY" was allocated in the first available *hole* within the heap, and not at the end.*

Now, we have successfully allocated the string "DYNAMIC MEMORY ALLOCATOR".

## Usage
You can simply compile and run `step4.c` in your environment. The program waits for a string input from the user to allocate.
