#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
#include<unistd.h>

#define NORMAL_EXIT 0
#define READ_BUFFER 1024

int done_simulation = 0; // to busy wait the main thread till worker finshes


int main(int argc, char* argv[]){

    int option = 0;
    int n = 0;
    int tlb_sz = 0;
    int lvls = 0;
    int arr_ind; // argument index to find array of bits
    char* log_mode= "summary";
    char* filename = NULL;



    FILE* file;


    // Proces the mandatory arguments first

    while ((option = getopt(argc, argv, "n:c:o:")) != -1) { // identify if alpha is specified through command line
        switch (option) {
            case 'n':
                n = strtof(optarg, NULL);  // number of addresses to process
                if (n <= 0){
                    printf("Number of memory accesses must be a number, greater than 0");
                    exit(NORMAL_EXIT);
                }
                break;
            case 'c': // the tlb cache size
                tlb_sz = strtof(optarg, NULL);
                if (tlb_sz <0){
                    printf("Cache capacity must be a number, greater than or equal to 0");
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

    for (int i = optind; i < argc; i++) {
        if (filename == NULL) {
            filename = argv[i];  // Assuming first non optional arg is filename, according to online sources
            if(argv[i+1] == NULL){
                printf("page table needs page levels\n");
                exit(NORMAL_EXIT);
            }
            lvls = NumLvl(argv[i+1]); // number of levels
            arr_ind = i+1;
        } else {
            printf("additional argument detected: %s\n", argv[i]);
            exit(NORMAL_EXIT);
        }
    }

// If file name is not found 
    if (filename == NULL) {
        printf("No file detected.\n");
        exit(NORMAL_EXIT);
    }

// Read the contents from the file

    file = fopen(filename, "r"); // open the file using the path

    // Check for file validity and permissions
    if(file == NULL){
        printf("Unable to open <<%s>>", filename);
        exit(NORMAL_EXIT);
    }else{
        printf("File name: <<%s>>\n", filename); //debug statement
    }
    // Get in the structure specifics


    int* treeScheme = processString(argv[arr_ind], lvls); // array of bit counts per level


    // Base on the given scheme calculate masks and shift sizes for each level
    unsigned int* masks = GenerateMask(lvls, treeScheme);
    unsigned int* shiftSizes = GetShiftSizes(treeScheme, lvls);
    unsigned int* ptrArraySizes = (unsigned int*)malloc(sizeof(unsigned int)*lvls); // the array size for each page level

    if(ptrArraySizes == NULL){
        perror("Memory allocation failed\n");
    }
    // Explicit declaration of ptrArray
    for(int i = 0; i < lvls; i++){
        ptrArraySizes[i] = 0;
    }
    // Get the entry count size for each level
    for(int i = 0; i < lvls; i++){
        // 1 << x, where x is an intiger value, resluts in 1*(2^x)
        // Hence in our case 1 << (number of bits assigned for a level) will yield 2^bits or the size of nextPtrarray
        ptrArraySizes[i] = 1 << treeScheme[i]; 

    }

    

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