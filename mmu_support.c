#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mmu_support.h"

/////////////////////////////////////////////////////////////////////
// various constant declarations used through several source files//
///////////////////////////////////////////////////////////////////

const int page_size = 256;
const int frame_size = 256;
const int tlb_size = 16;

const int MASK_FOR_PAGE_NUM = 0xFF00;
const int MASK_FOR_OFFSET = 0x00FF;
const int INVALID = -1;

/////////////////////////////////////////////////////////////////////

// extract the required page bits from logical address
int extract_page_number(int logical_address)
{
    int page_num = logical_address & MASK_FOR_PAGE_NUM;
    page_num = page_num >> 8;

    return page_num;
}

// extract the require offset bits from logical address
int extract_offset(int logical_address)
{
    int offset = logical_address & MASK_FOR_OFFSET;

    return offset;
}

// make the page table
PageTable *generate_page_table(int passed_size)
{
    PageTable *temp = malloc ( sizeof ( PageTable ) );

    (*temp).num_pFaults = 0;
    (*temp).size = passed_size;

    (*temp).pages = malloc ( sizeof(int) * passed_size );
   

    for ( int iter = 0; iter < passed_size; iter++)
    {
        (*temp).pages[iter] = INVALID;
    }

    return temp;

}

/*void update_page_table(PageTable *page_table, int page_num)
{
    page_table -> pages[page_num] = 3;
}*/

// make the tlb
TLB *generate_tlb(int passed_size)
{
    TLB *temp = malloc ( sizeof ( TLB ) );

    (*temp).size = passed_size;

    (*temp).page_nums = malloc ( sizeof(int) * passed_size );
    (*temp).frame_nums = malloc ( sizeof(int) * passed_size );

    (*temp).tlb_hits = 0;

    for ( int iter = 0; iter < passed_size; iter++)
    {
        (*temp).page_nums[iter] = 0;
    }

    return temp;
}

Memory *generate_memory(int passed_size)
{
    Memory *temp = malloc ( sizeof ( Memory ) );

    (*temp).size = passed_size;

    (*temp).frames = malloc ( sizeof(int) * passed_size );

    return temp;
}

unsigned int bitwiseOperations(int iter, int iter2, int inp)
{
    unsigned int num_bits = 8 * sizeof(inp);
    
    int iter3 = 0;
    int mask = 0;
    for(iter3 = 0; iter3 < num_bits; iter3++){
        if(iter <= iter3 && iter3 <= iter2){
            mask += 1 << iter3;
        }
    }

    return ((unsigned int) inp & mask) >> iter;
}
