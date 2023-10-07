#include <vector>
#include <iostream>
#include <tuple>
#include <cstring>
#include "buffer_pool.hpp"
#include "definitions.hpp"

using namespace BufferPool;
using namespace std;

BufPool::BufPool(size_t bufPoolSize, size_t blkSize)
{
    this->bufPoolSize = bufPoolSize;
    this->blkSize = blkSize;

    this->bufPoolUsedBlks = 0;
    this->bufPoolUsedRecords = 0;
    this->curBlkSizeUsed = 0;
    this->numAllocBlks = 0;
    this->numRecords = 0;
    this->numAvailBlks = bufPoolSize / blkSize;

    //create memory pool of blocks and initialize all to NULL.
    this->memPoolPtr = operator new(bufPoolSize);
    std::memset(memPoolPtr, '\0', bufPoolSize);
    this->blkPtr = nullptr;

}

BufPool::~BufPool()
{
}

size_t BufPool::getBlkSize()
{
    return blkSize;
}

size_t BufPool::getBufPoolSize()
{
    return bufPoolSize;
}

size_t BufPool::getMemPoolUsedBlks()
{
    return bufPoolUsedBlks;
}

size_t BufPool::getMemPoolUsedRecords()
{
    return bufPoolUsedRecords;
}

size_t BufPool::getCurBlkSizeUsed(){
    return curBlkSizeUsed;
}

void *BufferPool::BufPool::getmemPtr()
{
    return memPoolPtr;
}

int BufPool::getNumAllocBlks()
{
    return numAllocBlks;
}

int BufPool::getNumAvailBlks()
{
    return numAvailBlks;
}

int BufPool::getNumRecords()
{
    return numRecords;
}

bool BufPool::allocateBlk() {       // check if there are still avail blocks & allocates block to buffer pool if true
    if (numAvailBlks > 0) {
        blkPtr = memPoolPtr + (numAllocBlks * blkSize);     // increment blkPtr by number of allocated blocks
        bufPoolUsedBlks += blkSize;
        numAvailBlks -= 1;          // decrement the available number of blocks by 1
        numAllocBlks += 1;          // increment the number of allocated blocks by 1
        curBlkSizeUsed = 0;
        return true;
    }
    else {
        cout << "MEMORY FULL";
        return false;
    }
}

// Since records are same size & large number of records to insert, easier to insert records not in sequence
tuple<void *, uint> BufPool::writeRecord(uint recordSize) {
    if(blkSize < (curBlkSizeUsed + recordSize) || numAllocBlks == 0){// check if block has no space to insert record or if there are no existing blocks in buffer pool
        if (!allocateBlk())
            throw "Unable to reserve space as no free space in blocks or no blocks can be allocated";
    }

    tuple<void *, uint> recordAddress(blkPtr, curBlkSizeUsed);      // tuple to contain block pointer and address offset to insert record

    bufPoolUsedRecords += recordSize;   // update size of buffer pool that is used by records
    curBlkSizeUsed += recordSize;       // update size of block that is used
    numRecords += 1;

    return recordAddress;
}

bool BufPool::deleteRecord(void *blockAddress, uint relOffset, std::size_t deletionSize){
    try {
        // remove block by setting values to null
        void *deletionAddress = (char *)blockAddress + relOffset;
        std::memset(deletionAddress, '\0', deletionSize);

        bufPoolUsedRecords -= deletionSize;

        //check if block is empty
        uchar tempBlk[blkSize];
        std::memset(tempBlk, '\0', blkSize);
        numRecords -= 1;
        if (memcmp(blockAddress, tempBlk, blkSize)==0){
            bufPoolUsedBlks -= blkSize;
            numAllocBlks--;
            numAvailBlks++;
        }

        return true;

    }
    catch (...){
        cout << "Error: Unable to delete data found at memory address " << blockAddress << " with relative offset " << relOffset << "." << '\n';
        return false;
    }
}