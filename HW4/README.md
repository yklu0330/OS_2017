## HW4: Designing a Virtual Memory Manager

### Goal
- Simulate the steps of translating logical addresses to physical addresses using translation look-aside buffers (TLB) and page table.
- Your program need to read logical addresses. Then, use a TLB and a page table to translate logical addresses to the corresponding physical addresses and output translated physical addresses and the byte stored at the physical memory.

### Input
BACKING_STORE.bin
- A binary file of size 65536 (256*256) bytes.
- Represent the backing store which store parts of pages.
- When a page fault occurs, you need to read the correspondingly bytes and move to physical memory, updating TLB and page table.

address.txt
- Include n logical addresses.
- First line implies the total number of logical addresses.
- N = [20, 3000]

### Output
results.txt
- Each line consists of physical address and value according to addresses.
- Last two line output the number of TLB hits and Page faults.
- The output format must be same as the example TAs given and name it as “results.txt”.

### Run
- Compile:
`g++ -std=c++11 0416025_hw4.cpp`
- Execute:
`./a.out BACKING_STORE.bin address.txt`