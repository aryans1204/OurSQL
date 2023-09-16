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
    this->totalBlkSizeUsed = 0;
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

size_t BufPool::getTotalBlkSizeUsed()
{
    return totalBlkSizeUsed;
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