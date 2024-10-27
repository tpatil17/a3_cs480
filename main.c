#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
#include<unistd.h>
#include "BitMasker.h"

#define NORMAL_EXIT 0
#define READ_BUFFER 1024
#define max_bits 28

int done_simulation = 0; // to busy wait the main thread till worker finshes


int main(int argc, char* argv[]){

    int option = 0;
    int n = 0;
    int tlb_sz = 0;
    int lvls = 0;
    int* bits_arr = (int*)malloc(sizeof(int)*max_bits); // arr of bits, the max levels allowed is 28 since sum of bits cant exceed 28
    for(int i = 0; i < max_bits; i+=1){
        bits_arr[i] = 0;
    }
    char* log_mode= "summary";
    char* filename = NULL;



    FILE* file;


    // Proces the mandatory arguments first

    while ((option = getopt(argc, argv, "n:c:o:")) != -1) { // identify if alpha is specified through command line
        switch (option) {
            case 'n':
                n = strtof(optarg, NULL);  // number of addresses to process
                if (n <= 0){
                    printf("Number of memory accesses must be a number, greater than 0\n");
                    exit(NORMAL_EXIT);
                }
                break;
            case 'c': // the tlb cache size
                tlb_sz = strtof(optarg, NULL);
                if (tlb_sz <0){
                    printf("Cache capacity must be a number, greater than or equal to 0\n");
                    exit(NORMAL_EXIT);
                }
                break;
            case 'o': // the tlb cache size
                log_mode = optarg;
                
                break;

            default:
                n = 1;
                break;
        }
    }

// Debug statements to print values of optional arguments
        printf("the entered value for n: %u\n", n);
        printf("the entered value for c: %u\n", tlb_sz);
        printf("the entered value for o: %s\n", log_mode);    

    filename = argv[optind];
    optind+=1; // to the bit array
    int lvl_ctr = 0;
    while(argv[optind] != NULL){
        bits_arr[lvl_ctr] = atoi(argv[optind]);
        printf("%d\n", argv[optind]);
        optind+=1;
        lvl_ctr+=1;
    }
    lvls = lvl_ctr;// calculate the depth

// If file name is not found 
    if (filename == NULL) {
        printf("No file detected.\n");
        exit(NORMAL_EXIT);
    }
    if(bits_arr == NULL){
        printf("we need levels to procees\n");
        exit(NORMAL_EXIT);
    }

// Print the file name and depth and array

    printf("File name: %s\n", filename);
    printf("arr of bits : ");
    for(int i =0; i < lvl_ctr; i+=1){
        printf("%d",bits_arr[i]);
    }
    printf("\n");

    printf("Depth of the tree: %d\n", lvls);

// Read the contents from the file

    file = fopen(filename, "r"); // open the file using the path

    // Check for file validity and permissions
    if(file == NULL){
        printf("Unable to open <<%s>>\n", filename);
        exit(NORMAL_EXIT);
    }else{
        printf("File name: <<%s>>\n", filename); //debug statement
    }
    // Get in the structure specifics


//     char buffer[READ_BUFFER];
//     char copy_buff[READ_BUFFER];
//     char* token = NULL;
//     unsigned int numProc = 0; // how many processes or lines do we have
    
// // count the number of lines

//     while (fgets(buffer, READ_BUFFER, file) != NULL) {

//         numProc += 1;
//     }

//     // Create a table to store the burst time for processes
return 0;
}