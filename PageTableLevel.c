//************************************************************************
// Name: Tanishq Patil
// RED ID: 132639686
// Class: CS 480
// Assignment 1
//************************************************************************
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "PageTableLevel.h"


// The Fail safe, to prevent abrupt system closure
void FailSafe(){
    perror("Failed to allocate Memory");
    exit(EXIT_FAILURE);
}



// create an empty page table
PageTable startPageTable(int lvls){

    PageTable root;
    root.levelCount = lvls;
    root.bitMasks = (unsigned int*) malloc(sizeof(unsigned int)*lvls);
    if(root.bitMasks == NULL){
        FailSafe();
    }
    for (int i = 0; i < lvls; i++) {
        root.bitMasks[i] = 0;
    }
    //********************************************
    // Explicitly declare entry count array
    root.entryCount = (unsigned int*) malloc(sizeof(unsigned int)*lvls);
    if(root.entryCount == NULL){
        FailSafe();
    }
    for (int i = 0; i < lvls; i++) {
        root.entryCount[i] = 0;
    }
    //********************************************
    root.shift_array = (unsigned int*) malloc(sizeof(unsigned int)*lvls);
    if(root.shift_array == NULL){
        FailSafe();
    }
    for (int i = 0; i < lvls; i++) {
        root.shift_array[i] = 0;
    }
    //********************************************
    root.zeroPage = (PageLevel*)malloc(sizeof(PageLevel)); // Initially will point to void
    if(root.zeroPage == NULL){
        FailSafe();
    }
    //root.zeroPage = NULL; // initially points to nothing
    return root; // Page Table is the root
}

// Create a new page level
PageLevel* startPageLevel(int Lvl, PageTable* root, unsigned int arr_size){

    PageLevel* level = (PageLevel*)malloc(sizeof(PageLevel)); // page level object

    level->lvl = Lvl;

    //allocate memory for root pointet and point it to the root table
    level->root = root;

    //********************************************

    level->numAccess = 0; // default access to zero

    //********************************************

    level->NextLevelPtr = (PageLevel**)malloc(sizeof(PageLevel*)*arr_size);
    if (level->NextLevelPtr == NULL){
        FailSafe();
    }
    for(int i = 0; i < arr_size; i++){
        level->NextLevelPtr[i] = NULL;
    }

    return level; // return the PageLevel object with all set to NULL, this only allocates memory

}

// Function to return the page number for each level given the mask

unsigned int extractPageNumberFromAddress(unsigned int PageMask, unsigned int shiftSize, unsigned int Addr){

    unsigned int page;

    page = (Addr & PageMask) >> shiftSize;

    return page;

}

// Traverse tree for a given page number, create new levels if not alredy created
// Function below checks if at a given level the page it needs to go to is null, if null a new level is created

unsigned int recordPageAccess(unsigned int addr, PageLevel* pgLvl){

    //update the level attributes for each level
    //current level in case we need to create a page
    int curLvl = pgLvl->lvl;

    // pointer to the root or the Page Table
    PageTable* rootPtr = pgLvl->root;

    if(curLvl == rootPtr->levelCount){
        pgLvl->numAccess++;

        return pgLvl->numAccess;
    }

    //
    //
    else{
        unsigned int curPageMask = pgLvl->root->bitMasks[pgLvl->lvl];

        
        unsigned int curPageShift = pgLvl->root->shift_array[pgLvl->lvl];


        unsigned int pgNum = extractPageNumberFromAddress(curPageMask, curPageShift, addr);
        // check if the page entry is null


        
        if (pgLvl->NextLevelPtr == NULL) {
        fprintf(stderr, "Error: pgLvl or NextLevelPtr is NULL\n");
        return 1;
        }

        PageLevel* nxtLevel = pgLvl->NextLevelPtr[pgNum]; //get the pointer to next level
        if ( nxtLevel != NULL){

            return recordPageAccess(addr, nxtLevel); //repeat till next level 
        } 
        else{
        
            // create a new page
            unsigned int ptrArraySize;
            if(curLvl < rootPtr->levelCount-1){
                ptrArraySize = pgLvl->root->entryCount[curLvl+1];
                //printf("Current level: %d\n", curLvl);
            }
            else{
                ptrArraySize = 0;
            }
            

            PageLevel* newPage;
            newPage = startPageLevel(curLvl+1, rootPtr, ptrArraySize);


            nxtLevel = newPage;
            pgLvl->NextLevelPtr[pgNum] = newPage;

            if(newPage->lvl == rootPtr->levelCount){
                // we have reached the deepest level
                newPage->numAccess++;
                return newPage->numAccess;
            }
            else{
                return recordPageAccess(addr, newPage);
            }

        }
    }
}

// Function to return an array of indeces
unsigned int* pageIndice(unsigned int* PageMasks, unsigned int* shiftSizes, unsigned int Addr, int lvls){

    // allocate memory to store the index array
    unsigned int* retArr = (unsigned int*)malloc(sizeof(unsigned int)*lvls);
   

    for(int i = 0; i < lvls; i++){
        retArr[i] = extractPageNumberFromAddress(PageMasks[i], shiftSizes[i], Addr);
    }
    return retArr;
}