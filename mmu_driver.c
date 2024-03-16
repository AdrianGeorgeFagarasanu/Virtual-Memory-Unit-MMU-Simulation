#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "mmu_support.h"

/////////////////////////////
// file associated content//
///////////////////////////

unsigned char tlb[16][2] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned char available = 0;

unsigned char pgTbl[256] = {0};

bool pgTblOk[256] = {false};

unsigned int pgTblNum[256] = {0};

bool ocuppiedFrame[256] = {false};
bool ocuppiedFrame_small[128] = {false};

char mem[256][256];

int accessed = 0;
int pgFlts = 0;
int tlbHits = 0;
float tlbHitsRate = 0;
float pgFltsRate = 0;

/////////////////////////////

int system(const char *command);


unsigned char calculateLRU(bool * physical_mem)
{
    unsigned int iter = 0;
    
    unsigned int tiny = pgTblNum[0];
    unsigned int tinyI = 0;


    for(iter = 0; iter < 256; iter++)
    {
        if(pgTblOk[iter])
        {
            tiny = pgTblNum[iter];
            tinyI = iter;
        }
    }


    for(iter = 0; iter < 256; iter++)
    {
        if(pgTblNum[iter] < tiny)
        {
            if(pgTblOk[iter])
            {

                tiny = pgTblNum[iter];
                tinyI = iter;
            }
        }
    }
    
    unsigned char page = tinyI;
    
    unsigned char frame = pgTbl[page];

    pgTblOk[page] = false;

    *(physical_mem + frame) = false;

    return frame;
    
}

int ifPageFault(unsigned char page, unsigned char frame, char * backing_store, bool * physicalMem)
{
    int z;
    int iter; 
    int last;
    FILE *fp;

    fp = fopen(backing_store,"rb"); 

    fseek(fp, 256 * (int) page, SEEK_SET);

    for (iter = 0, last = 256; iter < last && (z = fgetc(fp)) != EOF; iter++)
    {
     
        mem[frame][iter] = z;

    }
    
    *(frame + physicalMem) = true;
    
    fclose(fp);

    return 0;
}

unsigned char lookForAvailableFrame(unsigned int last, bool * physicalMem)
{
    unsigned char iter;

    for(iter = 0; iter < last; iter++)
    {
        if(*(physicalMem + iter) == false)
        {
            return iter;
        }
    }
    return calculateLRU(physicalMem);
}

void findRates(bool tlb_hit, bool page_fault)
{
    
    accessed++;

    if(page_fault) pgFlts++;
    if(tlb_hit) tlbHits++;

    pgFltsRate = ((float) pgFlts * 100)  / accessed;
    tlbHitsRate = ((float) tlbHits * 100)  / accessed;
    
}

int addToFile(bool large_size, unsigned int addLog, unsigned int addP, int val)
{
    FILE * fp;
    char outputFileName[] = "output128.csv";
    
    if(large_size)
    {
        strcpy(outputFileName, "output256.csv");
    }

    fp = fopen(outputFileName, "a");

    fprintf(fp, "%u,%u,%d\n", addLog, addP, val); 

    fclose(fp);

    return 0;

}

int addRates(bool large_size, float pgFltsRate, float tlbHitsRate)
{
    FILE * fp;
    char outputFileName[] = "output128.csv";

    if(large_size)
    {
        strcpy(outputFileName, "output256.csv");
    }

    fp = fopen(outputFileName, "a");
    
    fprintf(fp, "Page Faults Rate, %.2f%%,\n", pgFltsRate);
    fprintf(fp, "TLB Hits Rate, %.2f%%,", tlbHitsRate);

    fclose(fp);
    
    return 0;
}



int main(int argc, char *argv[]){

    bool large_size = strcmp(argv[1], "256") == 0;



    if(large_size)
    {
        short unsigned int address;

        FILE * fp;
        char * li = NULL;
        
        size_t size = 0;
        size_t scan;

        fp = fopen(argv[3], "r");

        while ((scan = getline(&li, &size, fp)) != -1) 
        {
            
            address = (short unsigned int) atoi(li);
            

            unsigned char offset = (unsigned char) bitwiseOperations(0, 7, address);
            unsigned char page = (unsigned char) bitwiseOperations(8, 15, address);

            signed char iTLB = -1;

            int iter;
            for (iter = 0; iter < 16; iter++)
            {
                if(tlb[iter][0] == page)
                {
                    iTLB = iter;
                }
            }

            unsigned char frame = 0;

            if(iTLB != -1)
            {
                frame = tlb[iTLB][1];
                
                findRates(true, false);

            }
            else
            {
                if(pgTblOk[page] == false)
                {
                    
                    frame = lookForAvailableFrame(256, ocuppiedFrame);

                    ifPageFault(page, frame, argv[2], ocuppiedFrame); 

                    pgTblOk[page] = true;
                    pgTbl[page] = frame;

                    tlb[available][1] = frame;
                    tlb[available][0] = page;

                    if(available==15)
                    {
                        available=0;
                    }
                    else
                    {
                        available++;
                    }

                    findRates(false, true);

                }
                else
                {    
                    frame = pgTbl[page];

                    tlb[available][1] = frame;
                    tlb[available][0] = page;

                    if(available==15)
                    {
                        available=0;
                    }
                    else
                    {
                        available++;
                    }

                    findRates(false, false);
                }
            }

            unsigned short int newFrame = ((unsigned short int) frame) << 8;
            unsigned short int addP = newFrame | ((unsigned short int) offset);

            char memData = 0;
            memData = mem[frame][offset];

            addToFile(true, address, addP, memData);

        }

        addRates(true, pgFltsRate, tlbHitsRate);

        fclose(fp);
        if (li)
        {
            free(li);
        }
    }

    else
    {
        unsigned int num = 0;

        short unsigned int address;

        FILE * fp;
        char * li = NULL;

        size_t size = 0;
        size_t scan;

        fp = fopen(argv[3], "r");

        while ((scan = getline(&li, &size, fp)) != -1) {
            
            address = (short unsigned int) atoi(li);
            

            unsigned char offset = (unsigned char) bitwiseOperations(0, 7, address);
            unsigned char page = (unsigned char) bitwiseOperations(8, 15, address);
        
            
            signed char iTLB = -1;

            int iter;
            for (iter = 0; iter < 16; iter++)
            {
                if(tlb[iter][0] == page){
                    iTLB = iter;
                }
            }

            unsigned char frame = 0;

            if(iTLB != -1)
            {
                frame = tlb[iTLB][1];
                
                findRates(true, false);
            }
            else
            {
                if(pgTblOk[page] == false)
                {
                    
                    frame = lookForAvailableFrame(128, ocuppiedFrame_small);

                    ifPageFault(page, frame, argv[2], ocuppiedFrame_small); 

                    pgTblOk[page] = true;
                    pgTbl[page] = frame;

                    tlb[available][1] = frame;
                    tlb[available][0] = page;

                    if(available==15)
                    {
                        available=0;
                    }
                    else
                    {
                        available++;
                    }

                    findRates(false, true);

                }
                else
                {    

                    frame = pgTbl[page];

                    tlb[available][1] = frame;
                    tlb[available][0] = page;

                    if(available==15)
                    {
                        available=0;
                    }
                    else
                    {
                        available++;
                    }

                    findRates(false, false);
                }
            }

            pgTblNum[page] = num; 

            unsigned short int newFrame = ((unsigned short int) frame) << 8;
            unsigned short int addP = newFrame | ((unsigned short int) offset);

            char memData = 0;
            memData = mem[frame][offset];

            addToFile(false, address, addP, memData);

            num++;

        }

        addRates(false, pgFltsRate, tlbHitsRate);

        fclose(fp);

        if (li)
        {
            free(li);
        }

    }

    return 0;
}