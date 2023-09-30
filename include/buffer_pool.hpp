#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H

#include <vector>
#include <iostream>
#include "definitions.hpp"

namespace BufferPool {


class BufPool{
private:
    void *memPoolPtr;
    void *blkPtr;

    std::size_t bufPoolSize;
    std::size_t blkSize;
    std::size_t bufPoolUsedBlks;
    std::size_t bufPoolUsedRecords;
    std::size_t curBlkSizeUsed;

    int numAllocBlks;
    int numAvailBlks;

public:
    BufPool(size_t bufPoolSize, size_t blkSize);

    ~BufPool();

    std::size_t getBlkSize();

    std::size_t getBufPoolSize();

    std::size_t getMemPoolUsedBlks();

    std::size_t getMemPoolUsedRecords();

    std::size_t getCurBlkSizeUsed();

    int getNumAllocBlks();

    int getNumAvailBlks();

    bool deleteRecord(void *blockAddress, uint relOffset, std::size_t deletionSize);

    bool allocateBlk();

    tuple<void *, uint> writeRecord(uint recordSize);

    bool deleteRecord(void *blockAddress, uint relOffset, std::size_t deletionSize);
};
};
#endif
