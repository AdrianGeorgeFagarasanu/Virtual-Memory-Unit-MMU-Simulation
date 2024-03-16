## Design and Implementation of a Virtual Memory Unit (MMU)

This program translates logical to physical addresses for a virtual address space of 
size 2<sup>16</sup> = 65,536 bytes. This program reads from a file containing logical addresses and, using a TLB and a page 
table, translates each logical address to its corresponding physical address and output the value of the byte stored 
at the translated physical address. This includes resolving page faults using demand paging, managing a TLB, and 
implementing a page-replacement algorithm.

### More Specifications   
This program will read a file containing several 32-bit integer numbers that represent logical addresses. Only 16-bit addresses.

Other specifics include the following:
* 2<sup>8</sup> entries in the page table 
* Page size of 2<sup>8</sup> bytes 
* 16 entries in the TLB 
* Frame size of 2<sup>8</sup> bytes 
* 256 frames 
* Physical memory of 65,536 bytes (256 frames × 256-byte frame size)    

### Phase 1 
#### Address Translation

Will translate logical to physical addresses using a TLB and page table as outlined in Section 9.3. First, 
the page number is extracted from the logical address, and the TLB is consulted. In the case of a TLB hit, the frame
number is obtained from the TLB. In the case of a TLB miss, the page table must be consulted. In the latter case, either
the frame number is obtained from the page table, or a page fault occurs.


#### Handling Page Faults
Will implement demand paging as described in Section 10.2. The backing store is represented by the file 
`BACKING_STORE.bin`, a binary file of size 65,536 bytes. When a page fault occurs, read in a 256-byte page from 
the file BACKING STORE and store it in an available page frame in physical memory. For example, if a logical address with 
page number 15 resulted in a page fault, this program will read in page 15 from BACKING STORE and store it in a page frame in physical memory. Once this frame is stored (and 
the page table and TLB are updated), subsequent accesses to page 15 will be resolved by either the TLB or the page table.


### Phase 2 
#### Page Replacement
Physical memory is typically much smaller than a virtual address space. This phase of the program now 
assumes using a smaller physical address space with 128 page frames rather than 256. So at this phase, we have at most
2<sup>7</sup> valid entries in the page table (i.e., 128 pages). This change will require modifying 
your program so that it keeps track of free page frames as well as implementing a page-replacement policy using 
LRU (Section 10.4) to resolve page faults when there is no free memory.

