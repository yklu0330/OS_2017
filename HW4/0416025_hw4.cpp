#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <deque>
#include <algorithm>

using namespace std;

int logicAddr;
int pageNum;

struct tlb{
    int page, frame;
};

bool IsPage(struct tlb t)
{
    return (t.page == pageNum);
}

int main(int argc, char const *argv[])
{
    FILE *filePtr1, *filePtr2;
    filePtr1 = fopen(argv[1], "rb"); //BACKING_STORE
    filePtr2 = fopen(argv[2], "r"); //address.txt

    deque<struct tlb> tlb_que;
    deque<struct tlb>::iterator iter;
    int pageTable[256];
    for (int i = 0; i < 256; i++)
        pageTable[i] = -1;
    signed char physicalMem[256 * 256];
    for (int i = 0; i < 256 * 256; i++)
        physicalMem[i] = 0;

    int physicalIdx = 0;

    int tlbHits = 0;
    int pageFault = 0;

    int testNum;
    fscanf(filePtr2, "%d", &testNum);
    for (int i = 0; i < testNum; i++)
    {
        fscanf(filePtr2, "%d", &logicAddr);
        pageNum = logicAddr >> 8;
        iter = find_if(tlb_que.begin(), tlb_que.end(), IsPage);
        if (iter != tlb_que.end()) //tlb hit
        {
            //convert to physical address
            int physicalAddr = iter->frame * 256 + logicAddr % 256;
            cout << physicalAddr << " " << (int)physicalMem[physicalAddr] << endl;
            
            //move to back of the deque
            struct tlb temp;
            temp.page = iter->page;
            temp.frame = iter->frame;
            tlb_que.erase(iter);
            tlb_que.push_back(temp);
            tlbHits++;
        }
        else //tlb miss
        {
            int frameNum = pageTable[pageNum];
            if (frameNum == -1) //page fault
            {
                //update physical memory
                fseek(filePtr1, pageNum * 256, SEEK_SET);
                fread(physicalMem + physicalIdx * 256, 256, 1, filePtr1);

                //update page table
                pageTable[pageNum] = physicalIdx;

                //update TLB
                struct tlb temp;
                temp.page = pageNum;
                temp.frame = physicalIdx;
                if (tlb_que.size() >= 16)
                    tlb_que.pop_front();  //pop the victim page
                tlb_que.push_back(temp);

                //print result
                int physicalAddr = physicalIdx * 256 + logicAddr % 256;
                cout << physicalAddr << " " << (int)physicalMem[physicalAddr] << endl;

                physicalIdx++;

                pageFault++;
            }
            else //page table match
            {
                //update TLB
                struct tlb temp;
                temp.page = pageNum;
                temp.frame = logicAddr % 256;
                if (tlb_que.size() >= 16)
                    tlb_que.pop_front();  //pop the victim page
                tlb_que.push_back(temp);

                //print result
                int physicalAddr = frameNum * 256 + logicAddr % 256;
                cout << physicalAddr << " " << (int)physicalMem[physicalAddr] << endl;
            }
        }
    }

    cout << "TLB hits: " << tlbHits << endl;
    cout << "Page Faults: " << pageFault << endl;

    fclose(filePtr1);
    fclose(filePtr2);
    return 0;
}