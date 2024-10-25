//************************************************************************
// Name: Tanishq Patil
// RED ID: 132639686
// Class: CS 480
// Assignment 1
//************************************************************************
#ifndef PAGETABLELEVEL_H
#define PAGETABLELEVEL_H

// Header file for Page Table struct, Page level and related functions

// declare a reference to PageLevel
typedef struct PageLevel PageLevel;

//********************************************
typedef struct PageTable{

    int levelCount; // specified number of levels, based on command line prompt
    unsigned int *bitMasks; // pointer to an array of bitmasks
    unsigned int *shift_array; //bits to shift at each level
    unsigned int *entryCount; //number of entries at each level
    PageLevel* zeroPage; // pointer to the first pagelevel structure (level 0)

}PageTable;

//********************************************


struct PageLevel{

    int numAccess; // number of time the level is accessed
    int lvl; // the current level
    PageLevel** NextLevelPtr; // array of pointers to next level(depth) of pageLvl
    PageTable* root; // pointer to the root/PageTable, convinient to reference attributes like bitmasks

};


//********************************************


// Initializers for the structs above

void FailSafe(); // In case of errors, grace fully fail. In case of memory allocation

PageTable startPageTable(int lvls); // For the Page Table, arguments are the number of levels

PageLevel* startPageLevel(int Lvl, PageTable* root, unsigned int arr_size); // access to root, page number to mark in nextlvlptr

// Calculate page for a given level

unsigned int extractPageNumberFromAddress(unsigned int PageMask, unsigned int shiftSize, unsigned int Addr);

// Traverse tree for a given page number, create new levels if not alredy created
// Function below checks if at a given level the page it needs to go to is null, if null a new level is created

unsigned int recordPageAccess(unsigned int addr, PageLevel* pgLvl);

// Function that returns an array of page indices given an address and levels

unsigned int* pageIndice(unsigned int* PageMasks, unsigned int* shiftSizes, unsigned int Addr, int lvls);


#endif
