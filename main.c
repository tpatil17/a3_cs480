#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
#include<unistd.h>
#include "BitMasker.h"
#include "PageTableLevel.h"
#include "log.h"
#include "tracereader.h"
#include<math.h>

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

// Fetch mandatory arguments and verify the input (error handling)
    filename = argv[optind];
    optind+=1; // to the bit array
    int lvl_ctr = 0;
    int total_bits = 0;
    while(argv[optind] != NULL){
        bits_arr[lvl_ctr] = atoi(argv[optind]);
        if(bits_arr[lvl_ctr] <= 0){
            printf("Level %d page table must be at least 1 bit\n", lvl_ctr);
            exit(NORMAL_EXIT);
        }
        total_bits+= bits_arr[lvl_ctr];
        optind+=1;
        lvl_ctr+=1;
    }
    lvls = lvl_ctr;// calculate the depth
    if(total_bits > max_bits){
        printf("Too many bits used in page tables\n");
        exit(NORMAL_EXIT);
    }


// If file name is not found 
    if (filename == NULL) {
        printf("No file detected.\n");
        exit(NORMAL_EXIT);
    }
    if(bits_arr == NULL){
        printf("we need levels to procees\n");
        exit(NORMAL_EXIT);
    }
// *********************************************************************

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

//**********************************************************************
// once the input has been processed, the following section will create
// a page table based on the bits per level information
//**********************************************************************

    int* treeScheme = bits_arr; // array of bit counts per level


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

    // Start the Page Table
    PageTable Root = startPageTable(lvls); 

    // Initialize all table values
    Root.levelCount = lvls;
    Root.bitMasks = masks;
    Root.shift_array = shiftSizes;
    Root.entryCount = ptrArraySizes;
    //printf("Root Table created with levels: %d\n", Root.levelCount);
    // if -o is specified and bitmasks is the log mode, log the bitmasks and exit
    if (strcmp(log_mode, "bitmasks")==0){
        //printf("if condition for bitmasks passed\n");
        log_bitmasks(lvls, Root.bitMasks);
        return 0;
    }

//sum of the bits in the bit array
int sum = 0;
for(int i = 0; i < lvls; i+=1){
    sum += bits_arr[i];
}
Root.page_size = (unsigned int)pow(2, 32-sum);

//**********************************************************************************************
// log bitmaks condition is take care off and a page table is created
// Following lines will read in the trace file and create pages
//**********************************************************************************************
    PageLevel* level0 = startPageLevel(0, &Root, Root.entryCount[0]);

    Root.zeroPage = level0;

    p2AddrTr mTrace;
    unsigned int vAddr;
    if(n == 0){
        unsigned int ctr = 0;
        // read all, the default value of n is set to 0 as a flag but when n is specified 0 will not be accepted
        while(NextAddress(file, &mTrace)){
            vAddr = mTrace.addr;

            if(strcmp(log_mode, "offset") == 0){
                hexnum(offset(sum, vAddr));
            }

            insert_vpn2pfn(&Root, vAddr); // asign a frame to a vpn that does not exist
            ctr+=1;
        }
        if(strcmp(log_mode, "summary") == 0){
            log_summary(Root.page_size, Root.cache_hit, Root.page_table_hit, ctr, Root.frame_count, Root.total_entry);
        }
    }else{
        int ctr = 0; // to count the number of addresses processed
        while(NextAddress(file, &mTrace) & (ctr < n)){
            vAddr = mTrace.addr;

            if(strcmp(log_mode, "offset") == 0){
                hexnum(offset(sum, vAddr));
            }
            insert_vpn2pfn(&Root, vAddr);
            ctr+=1;
        }
    }

    
return 0;
}