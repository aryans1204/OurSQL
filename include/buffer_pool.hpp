#include <vector>

using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

class BufPool{
private:
    uchar *memPoolPtr;
    uchar *blkPtr;

    uint bufPoolSize;
    uint blkSize;
    uint bufPoolUsedBlks;
    uint bufPoolUsedRecords;
    uint curBlkUsed;

    int numAllocBlks;
    int numAvailBlks;

public:
    BufPool(uint bufPoolSize, uint blkSize);

    ~BufPool();

    uint getBlkSize(){
        return blkSize;
    }

    uint getBufPoolSize();

    int getMemPoolUsedBlks();

    int getMemPoolUsedRecords();

    int getCurBlkUsed();

    int getNumAllocBlks();

    int getNumAvailBlks();
};
