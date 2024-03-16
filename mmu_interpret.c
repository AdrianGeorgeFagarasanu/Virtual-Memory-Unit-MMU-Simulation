#include <stdio.h>
#include <stdlib.h>
#include "mmu_support.h"

//-------------------------------------------------------------------------------------------------------------------------------//
// This source file is focused on the interpretative operations derived from the virtual addresses determined at 'driver' level //
//-----------------------------------------------------------------------------------------------------------------------------//

/*void interpret()
{


}*/

int is_in_TLB(TLB *tlb, int page_num) 
{

    for (int iter = 0; iter < ( (*tlb).size ); iter += 1)
    {
        
        // Move the found TLB entry to the front (FIFO)
        int temp_page = (*tlb).page_nums[iter];
        int temp_frame = (*tlb).frame_nums[iter];

        for (int j = iter; j > 0; j--) 
        {
            (*tlb).page_nums[j] = (*tlb).page_nums[j - 1];
            (*tlb).frame_nums[j] = (*tlb).frame_nums[j - 1];
        }

        (*tlb).page_nums[0] = temp_page;
        (*tlb).frame_nums[0] = temp_frame;

        // Increment TLB hits
        (*tlb).tlb_hits += 1;

        return 0;

    }

    return -1;
}

//void amend_TLB(TLB tlb, )
