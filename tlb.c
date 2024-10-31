

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

    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Function to add a node to the back of the queue
void push(Cache* cache, Node* node) {

    if(cache->capacity == 0){

        return;
    }

    if(cache->size == cache->capacity){
        //the cache is full

        Node* temp = cache->head;
        cache->head = cache->head->next;
        pop(cache, temp);// toss the least recently used value out
        // in our cache the oldest value is at the top


    }

    if(cache->tail == NULL){
        //printf("first entry in the cache\n");
        cache->head = node;
        cache->tail = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else{

        cache->tail->next = node;
        node->prev = cache->tail;
        cache->tail = node;
        node->next = NULL; // no one after the tail

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
        if(jump == 1){
            //cache hit
            // swap the current node to the end and update the latest used vpn
     
            pop(cache, temp);
            push(cache, temp);
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}

int compArr(unsigned int* arr1, unsigned int* arr2, unsigned int lvls){
    
    int ctr = 0;
    
    
    while(ctr < lvls){
        if(arr1[ctr] != arr2[ctr]){
         
            return 0;
            
        }
        ctr+=1;
       
    }

    return 1;
}