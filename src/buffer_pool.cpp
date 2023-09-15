#include "buffer_pool.hpp"

BufPool::BufPool(uint bufPoolSize, uint blkSize)
{
}

BufPool::~BufPool()
{
}

uint BufPool::getBufPoolSize()
{
    return bufPoolSize;
}

int BufPool::getMemPoolUsedBlks()
{
    return bufPoolUsedBlks;
}

int BufPool::getMemPoolUsedRecords()
{
    return bufPoolUsedRecords;
}

int BufPool::getCurBlkUsed(){
    return curBlkUsed;
}

int BufPool::getNumAllocBlks()
{
    return numAllocBlks;
}

int BufPool::getNumAvailBlks()
{
    return numAvailBlks;
}