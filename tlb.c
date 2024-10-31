

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"BitMasker.h"
#include"tlb.h"
#include"log.h"

/**
 * @brief Allocate memory for the map struct
 * 
 *  
 * @return Map* 
 */
Map* startMap(){
    Map* map = (Map*)malloc(sizeof(map));
    map->valid = 0;

    return map;
}
// Helper function to create a new node
Node* StartNode(unsigned int* vpn, Map* info) {
    Node* new = (Node*)malloc(sizeof(Node));
    new->vpn = vpn;
    new->info = info;
    new->next = NULL;
    new->prev = NULL;

    return new;
}

// Helper function to initialize the cache
Cache* createCache(int capacity){
    Cache* new = (Cache*)malloc(sizeof(Cache));
    new->capacity = capacity;
    new->head = NULL;
    new->tail = NULL;
    new->size = 0;
    new->hits = 0;

    return new;
}

// Function to pop a specific node from the queue
Node* pop(Cache* cache, Node* node) {
    //printf("node to pop has pf : %u\n", node->info->pfn);

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        // Node is head
        cache->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        // Node is tail
        cache->tail = node->prev;
    }

    // upadte the cache size
    cache->size--;
    printf("cache size %d\n", cache->size);
    printf("cache capacity %d\n", cache->capacity);
    return node;
}

// Function to add a node to the back of the queue
void push(Cache* cache, Node* node) {
    printf("cache size %d\n", cache->size);
    printf("cache capacity %d\n", cache->capacity);
    if(cache->capacity == 0){
        //printf("cache size is zero, no cache implemented\n");
        return;
    }

    if(cache->size == cache->capacity){
        //the cache is full
        //printf("cache is full\n");
        Node* temp = cache->head;
        cache->head = cache->head->next;
        pop(cache, temp);// toss the least recently used value out
        // in our cache the oldest value is at the top

    }

    if(cache->tail == NULL){
        cache->head = node;
        cache->tail = node;
    }
    else{
        cache->tail->next = node;
        node->prev = cache->tail;
        cache->tail = node;

    }

    cache->size+=1;
    return;

}

// Utility to print cache contents
Node* lookup_Cache(Cache* cache, unsigned int* vpn, unsigned int lvls) {
    
    Node* temp = NULL;
    temp = cache->head;
    while(temp != NULL){
        int jump = compArr(temp->vpn, vpn, lvls);
        if(jump == 0){
            //cache hit
            // swap the current node to the end and update the latest used vpn
            printf("match found in cache\n");
            Node* to_push = pop(cache, temp);
            push(cache, to_push);
            return temp;
        }
        temp = temp->next;
    }
    printf("entry not in cache\n");
    return NULL;
}

int compArr(unsigned int* arr1, unsigned int* arr2, unsigned int lvls){
    
    int ctr = 0;
    
    while(ctr < lvls){
        if(arr1[ctr] != arr2[ctr]){
            printf("match found\n");
            return 0;
            
        }
        ctr+=1;
        //printf("no match\n");
    }
    printf("match not found\n");
    return 1;
}