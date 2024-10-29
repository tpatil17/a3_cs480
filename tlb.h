#define TLB_H
#ifndef TLB_H

#include <stdio.h>
#include <stdlib.h>
#include "PageTableLevel.h"

// Node structure for doubly linked list
typedef struct Node {
    unsigned int vpn;
    Map* info;
    struct Node* prev;
    struct Node* next;
} Node;

// Cache structure
typedef struct Cache {
    Node* head;
    Node* tail;
    int capacity;
    int size;
} Cache;

// Helper function to create a new node
Node* StartNode(unsigned int vpn, Map* info) {

}

// Helper function to initialize the cache
Cache* createCache(int capacity) {

}

// Function to pop a specific node from the queue
Node* pop(Cache* cache, Node* node) {
    
}

// Function to add a node to the front of the queue (for LRU policy)
void addToFront(Cache* cache, Node* node) {

}

// Utility to print cache contents
void printCache(Cache* cache) {

}


#endif