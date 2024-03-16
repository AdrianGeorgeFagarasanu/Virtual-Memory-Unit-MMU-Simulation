#ifndef MMU_SUPPORT_H
#define MMU_SUPPORT_H

////////////////////////////////////////////////////////
// extending reach of constants accross source files //
//////////////////////////////////////////////////////

extern const int page_size;
extern const int frame_size;
extern const int tlb_size;

extern const  int MASK_FOR_PAGE_NUM;
extern const int MASK_FOR_OFFSET;
extern const int INVALID;

////////////////////////////////////////////////////////

// extract the required page bits from logical address
int extract_page_number(int logical_address);

// extract the require offset bits from logical address
int extract_offset(int logical_address);

// define PageTable type
typedef struct PageTable 
{
    int size;
    int *pages;
    int num_pFaults;

} PageTable;

// define TLB type
typedef struct TLB
{
    int size;
    int *page_nums;
    int *frame_nums;
    int tlb_hits;

} TLB;

typedef struct Memory
{
    int size;
    int *frames;
 
} Memory;

// make the page table
PageTable *generate_page_table(int passed_size);

// make the tlb
TLB *generate_tlb(int passed_size);

// make the memory
Memory *generate_memory(int passed_size);

void update_page_table(PageTable *page_table, int page_num);

//void interpret();

int is_in_TLB(TLB *tlb, int page_num);

unsigned int bitwiseOperations(int iter, int iter2, int inp);

#endif