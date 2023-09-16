#include <vector>
#include <iostream>
#include <tuple>
#include <cstring>
#include "buffer_pool.hpp"
#include "definitions.hpp"

using namespace std;

BufPool::BufPool(size_t bufPoolSize, size_t blkSize)
{
    this->bufPoolSize = bufPoolSize;
    this->blkSize = blkSize;

    this->bufPoolUsedBlks = 0;
    this->bufPoolUsedRecords = 0;
    this->curBlkSizeUsed = 0;
    this->numAllocBlks = 0;
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

int BufPool::getNumAllocBlks()
{
    return numAllocBlks;
}

int BufPool::getNumAvailBlks()
{
    return numAvailBlks;
}

bool BufPool::allocateBlk(){

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
        if (memcmp(blockAddress, tempBlk, blkSize)==0){
            bufPoolUsedBlks -= blkSize;
            numAllocBlks--;
        }

        return true;

    }
    catch (...){
        cout << "Error: Unable to delete data found at memory address " << blockAddress << " with relative offset " << relOffset << "." << '\n';
        return false;
    }
}