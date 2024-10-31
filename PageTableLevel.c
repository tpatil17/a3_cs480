//************************************************************************
// Group Assignment
// Member Names: Tanishq Patil, Elai Lopez
// RED IDs: 132639686 , 825365860 (in that order)
// Class: CS 480
// Assignment 3
//************************************************************************
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "PageTableLevel.h"
#include "BitMasker.h"
#include "log.h"
#include "tlb.h"


// The Fail safe, to prevent abrupt system closure
void FailSafe(){
    perror("Failed to allocate Memory");
    exit(EXIT_FAILURE);
}


// create an empty page table
PageTable startPageTable(int lvls){

    PageTable root;
    root.cache_hit_flag = false;
    root.table_hit_flag = false;
    root.frame_count =0;
    root.total_entry = 0;
    root.page_table_hit = 0;
    root.cache_hit = 0;
    root.levelCount = lvls;
    root.bit_sum = 0;
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
    //root->total_entry+=arr_size;
    if (level->NextLevelPtr == NULL){
        FailSafe();
    }
    for(int i = 0; i < arr_size; i++){
        level->NextLevelPtr[i] = NULL;
    }
    if(Lvl == root->levelCount-1){ // last level so gotta assign a pfn here
        level->map = startMap(); //allocate memory
        level->map->pfn = root->frame_count;
        
        
        level->map->valid = 1;
    }else{
        // just a level
        level->map = NULL;
    }

    return level; // return the PageLevel object with all set to NULL, this only allocates memory

}

// Function to return the page number for each level given the mask

unsigned int extractPageNumberFromAddress(unsigned int PageMask, unsigned int shiftSize, unsigned int Addr){

    unsigned int page;

    page = (Addr & PageMask) >> shiftSize;

    return page;

}

Map* lookup_vpn2pfn(PageTable* table, unsigned int vAddr, Cache* cache){
    int curLvl = 0; // starts from the root

    PageLevel* cur_pg = table->zeroPage;
    // look up the cache first
    unsigned int *ret = pageIndice(table->bitMasks, table->shift_array, vAddr, table->levelCount);
    Node* cache_info = lookup_Cache(cache, ret, table->levelCount);
   
    // the above is a list of ints vpn 
        // compare it to the vpn arr in cache
        if(cache_info!= NULL){

            table->cache_hit+=1;
            table->cache_hit_flag = true;
            return cache_info->info; // if hit return else move on
        }
        
        // if cache is missed
    table->cache_hit_flag = false;
    while(curLvl < table->levelCount){
        
        unsigned int ind = extractPageNumberFromAddress(table->bitMasks[curLvl], table->shift_array[curLvl], vAddr);
        
        if(cur_pg->NextLevelPtr[ind] == NULL){
            // if the vpn is not logged return NULL
            //printf("new vpn detected\n");
            table->table_hit_flag = false;
            return NULL;
        }
        cur_pg = cur_pg->NextLevelPtr[ind];// point the cursor to new page level
        curLvl+=1; // next level for each valid entry
    }// until the last level or node is reached
    // if the entry is logged before return a pointer to its map
    // if cache is missed but page is hit push the recent entry to the cache
    Node* new = StartNode(ret, cur_pg->map);

    push(cache,new);

   
    table->page_table_hit+=1;
    table->table_hit_flag = true;
    return cur_pg->map;


}

void insert_vpn2pfn(PageTable* table, unsigned int vAddr, Cache* cache){
    unsigned int *ret;
    ret = pageIndice(table->bitMasks, table->shift_array, vAddr, table->levelCount);

    if(lookup_vpn2pfn(table, vAddr, cache) == NULL){
        //create a new entry
        table->cache_hit_flag = false;
        table->table_hit_flag = false;
        int curLvl = 0; // start from root level
        PageLevel* cursor = table->zeroPage;
        
        while (curLvl < table->levelCount)
        {   
            unsigned int ind = extractPageNumberFromAddress(table->bitMasks[curLvl], table->shift_array[curLvl], vAddr);
            
            if(cursor->NextLevelPtr[ind] == NULL){
                // no entry for that index at that level
                PageLevel* new_page;
                if(curLvl == table->levelCount -1){
                    new_page = startPageLevel(curLvl, table, 0);
                    // add to cache
                    Node* new = StartNode(ret, new_page->map);
                    push(cache, new);
                   
                    table->total_entry += 0;
                    table->frame_count+=1;
                }else{
                    new_page = startPageLevel(curLvl, table, table->entryCount[curLvl+1]);
                    table->total_entry += table->entryCount[curLvl+1];
                    
                }
                
                cursor->NextLevelPtr[ind] = new_page;
                cursor = cursor->NextLevelPtr[ind];
                // increase the number of page table entries
                cursor->numEntries+=1;
                
            }else{
                // simply move on in levels
                cursor = cursor->NextLevelPtr[ind];
                
            }
            curLvl+=1;
        }
        
    }
    return;
}
// Traverse tree for a given page number, create new levels if not alredy created
// Function below checks if at a given level the page it needs to go to is null, if null a new level is created

// Function to return an array of indeces
unsigned int* pageIndice(unsigned int* PageMasks, unsigned int* shiftSizes, unsigned int Addr, int lvls){

    // allocate memory to store the index array
    unsigned int* retArr = (unsigned int*)malloc(sizeof(unsigned int)*lvls);
   

    for(int i = 0; i < lvls; i++){
        retArr[i] = extractPageNumberFromAddress(PageMasks[i], shiftSizes[i], Addr);
    }
    return retArr;
}



void va2pa(PageTable*table, unsigned int Vaddr, Cache* cache){
    // get the map of vpn to pfn
    Map* frm_info =lookup_vpn2pfn(table, Vaddr, cache);
    // convert frame number to physical addr

    unsigned int off = offset(table->bit_sum, Vaddr); // get the offset
    int of_bit = 32 - table->bit_sum;
    unsigned int physical_addr = (frm_info->pfn << of_bit) + off;
    log_virtualAddr2physicalAddr(Vaddr, physical_addr);
    
}

void vpn2pfn(PageTable* table, unsigned int Vaddr, unsigned int* arr, Cache* cache){
    Map* info = lookup_vpn2pfn(table, Vaddr, cache);
    
    log_pagemapping(table->levelCount, arr, info->pfn);


}
    
void va2pa_atc_ptwalk(PageTable* table, unsigned int Vaddr, Cache* cache, bool cach_flag, bool table_flag){
    Map* frm_info =lookup_vpn2pfn(table, Vaddr, cache);
    // convert frame number to physical addr

    unsigned int off = offset(table->bit_sum, Vaddr); // get the offset
    int of_bit = 32 - table->bit_sum;
    unsigned int physical_addr = (frm_info->pfn << of_bit) + off;
    log_va2pa_ATC_PTwalk(Vaddr, physical_addr, cach_flag, table_flag);
    table->cache_hit_flag = false;
    table->table_hit_flag = false; // reset to false
    return;
}