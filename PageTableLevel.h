//************************************************************************
// Group Assignment
// Member Names: Tanishq Patil, Elai Lopez
// RED IDs: 132639686 , 825365860 (in that order)
// Class: CS 480
// Assignment 3
//************************************************************************
#ifndef PAGETABLELEVEL_H
#define PAGETABLELEVEL_H

#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<unistd.h>
#include "tlb.h"

// Header file for Page Table struct, Page level and related functions


// declare a reference to PageLevel
typedef struct PageLevel PageLevel;

//********************************************
typedef struct PageTable{

    int levelCount; // specified number of levels, based on command line prompt
    bool cache_hit_flag; // false (set it hit to false)
    bool table_hit_flag; // false
    unsigned int frame_count; // starts at zero, increses every time a frame is allocated
    unsigned int *bitMasks; // pointer to an array of bitmasks
    unsigned int *shift_array; //bits to shift at each level
    unsigned int *entryCount; //number of entries at each level
    unsigned long total_entry; // the total page table entries created
    unsigned int page_size; // size of the page 
    unsigned int cache_hit; // number of times chache is hit
    unsigned int page_table_hit; // number of times a page table is hit
    int bit_sum;
    PageLevel* zeroPage; // pointer to the first pagelevel structure (level 0)

}PageTable;

//********************************************


struct PageLevel{

    int numAccess; // number of time the level is accessed
    unsigned int numEntries; // number of non null entries on each level
    int lvl; // the current level
    PageLevel** NextLevelPtr; // array of pointers to next level(depth) of pageLvl
    PageTable* root; // pointer to the root/PageTable, convinient to reference attributes like bitmasks
    Map* map;

};


//********************************************


// Initializers for the structs above

void FailSafe(); // In case of errors, grace fully fail. In case of memory allocation



PageTable startPageTable(int lvls); // For the Page Table, arguments are the number of levels

PageLevel* startPageLevel(int Lvl, PageTable* root, unsigned int arr_size); // access to root, page number to mark in nextlvlptr

// Calculate page for a given level

unsigned int extractPageNumberFromAddress(unsigned int PageMask, unsigned int shiftSize, unsigned int Addr);
/**
 * @brief Given a virtual address the function tries to return the frame number of a given virtual addr if any, if not returns null
 * 
 * @param table the page table containing all the information
 * @param vAddr the vpn to traverse
 * @return Map* pointer to a map with corresponding fram number
 */
Map* lookup_vpn2pfn(PageTable* table, unsigned int vAddr, Cache* cache);
/**
 * @brief Given a VPN if the entry does not exist create it and assign it a frame
 * 
 * @param table 
 * @param vAddr 
 * @param frame 
 */
void insert_vpn2pfn(PageTable* table, unsigned int vAddr, Cache* cache);
// Traverse tree for a given page number, create new levels if not alredy created
// Function below checks if at a given level the page it needs to go to is null, if null a new level is created

// Function that returns an array of page indices given an address and levels

unsigned int* pageIndice(unsigned int* PageMasks, unsigned int* shiftSizes, unsigned int Addr, int lvls);



void va2pa(PageTable* table ,unsigned int Vaddr, Cache* cache);

void vpn2pfn(PageTable* table, unsigned int Vaddr, unsigned int* arr, Cache* cache);

void va2pa_atc_ptwalk(PageTable* table, unsigned int Vaddr, Cache* cache, bool cach_flag, bool table_flag);
#endif
