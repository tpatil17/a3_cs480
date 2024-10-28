//************************************************************************
// Name: Tanishq Patil
// RED ID: 132639686
// Class: CS 480
// Assignment 1
//************************************************************************
#ifndef BITMASKER_H
#define BITMASKER_H

// Function generates a bit masks

unsigned int* GenerateMask(int lvls, int* arr);


// Calculate the shift size for each level

unsigned int* GetShiftSizes(int* arr, int lvls);

// process the string input to convert it into an array of ints
int* processString(char* arr, int lvls);

// count the number of levels specified
int NumLvl(char* arr);

unsigned int* offset(int* bit_arr, unsigned int vAddr, int lvls);
#endif