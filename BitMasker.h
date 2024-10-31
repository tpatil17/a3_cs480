//************************************************************************
// Group Assignment
// Member Names: Tanishq Patil, Elai Lopez
// RED IDs: 132639686 , 825365860 (in that order)
// Class: CS 480
// Assignment 3
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

unsigned int offset(int sum, unsigned int vAddr);
#endif