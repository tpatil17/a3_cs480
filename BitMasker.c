//************************************************************************
// Name: Tanishq Patil
// RED ID: 132639686
// Class: CS 480
// Assignment 1
//************************************************************************
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "BitMasker.h"


// Function to generate an array of masks give the tree scheme
//
unsigned int* GenerateMask(int lvls, int* arr){
    // for a 32 bit long address size, as is the standard for this assignment
    unsigned int base = 0xFFFFFFFF; // if all 32 bits were 1, its hex representation is 8 F's
    int carry = 0;
    unsigned int* maskArray = (unsigned int*)malloc(sizeof(unsigned int)*lvls);

    for(int i = 0; i < lvls; i++){
        maskArray[i] = 0;
    }


    for(int i = 0; i < lvls; i++){
        int pgSize = arr[i]; // the bits for the current level
        unsigned int mask = base >> carry; // to adjust for the front bits
        
        int shiftSize = 32 - (carry+pgSize); // 32 here is the number of bits in an address
       
        mask = mask >> shiftSize;
        mask = mask << shiftSize;
 
        maskArray[i] = mask;
        carry = carry+pgSize;
        
    }

    return maskArray;

}


// Calculates the shiftsize for each level

unsigned int* GetShiftSizes(int* arr, int lvls){
    unsigned int* shiftArray = (unsigned int*)malloc(sizeof(unsigned int)*lvls);
    //explicit declaration
    for(int i = 0; i < lvls; i++){
        shiftArray[i] = 0;
    }
 

    // Function Logic
    int carry = 0;

    for(int i = 0; i < lvls; i++){
        
        shiftArray[i] = 32 - (arr[i]+carry); // 32 is the number of bits in an address
        carry = carry + arr[i];
    }

    return shiftArray;

}

// process the string input to convert it into an array of ints
int* processString(char* arr, int lvls){
    

    int* retArr = (int*)malloc(sizeof(int)*lvls);
    if(retArr == NULL){
        perror("Memory allocation failed");
        exit(1);
    }
    char* token = strtok(arr, " ");
    int index = 0;

    while (token != NULL && index < lvls) {
        retArr[index++] = atoi(token);
        token = strtok(NULL, " ");
    }
    return retArr;
   
}

// count the number of levels specified
int NumLvl(char* arr){
    int count = 0;
    const char* tempStr = arr;
    while (*tempStr) {
        if (*tempStr == ' ') {
            count++;
        }
        tempStr++;
    }
    count++; // Add 1 to count for the last number

    return count;

}

unsigned int* offset(int* bit_arr, unsigned int vAddr, int lvls){
    int i = 0;
    int sum = 0; // to calculat the offset bits
    while(i < lvls){
        sum+= bit_arr[i];
        i+=1;
    }

    unsigned int base = 0xFFFFFFFF;
    //assuming a 32 bit address 
    int shift = 32 - sum;
    return (vAddr & (base >> shift));
}