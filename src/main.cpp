#include "buffer_pool.hpp"
#include "btree.hpp"
#include "record.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <tuple>

using namespace BufferPool;
using namespace std;

int main() {
    BufPool bufPool(104857600, 400);    //pool size = 100MB, block size = 400B

    BTree::BTree BTree(10);

    fstream newfile;

    vector<tuple<void *, uint>> dataset;

    cout << "<------------------- Data file read started ------------------->" << "\n" << "\n";

    newfile.open("games.txt",ios::in);  //open a file to perform read operation using file object
    if (newfile.is_open()){     //checking whether the file is open
        string tp;
        int count = 0;
        getline(newfile, tp);   //ignore first line
        while(getline(newfile, tp) /*&& count++ < 10*/){    //read data from file object and put it into string.
            cout << "next line\n";

            Record record;
            string tempLine;

            stringstream linestream(tp);
            getline(linestream, tempLine,'\t');
            linestream >> record.date >> record.teamID >> record.pts >> record.fg_pct >> record.ft_pct >> record.fg3_pct >> record.ast >> record.reb >> record.win;
            tuple<void *, uint> dataRecord = bufPool.writeRecord(sizeof(record));
            dataset.push_back(dataRecord);

            void *rcdAdr = (uchar *)get<0>(dataRecord) + get<1>(dataRecord);
            memcpy(rcdAdr, &record, sizeof(record));
            cout << rcdAdr << " " << record.teamID << '\n';
            BTree.insertRecord(record, record.fg_pct);
            cout << "main.cpp after insert: ";
            BTree.display();
        }

        cout << "<------------------- Data file read ended ------------------->" << "\n" << "\n";

        cout << "<------------------- Storage Statistics (Experiment 1) ------------------->" << "\n";
        cout << "1. Number of records: " << bufPool.getNumRecords() << "\n";
        cout << "2. Size of a record: " << sizeof(Record) << "\n";
        cout << "3. Number of records stored in a block: " << bufPool.getBlkSize()/sizeof(Record) << "\n";
        cout << "4. Number of allocated blocks: " << bufPool.getNumAllocBlks() << "\n\n";

        cout << "<------------------- B+ Tree Statistics (Experiment 2) ------------------->" << "\n";
        cout << "1. Parameter n of B+ tree: " << BTree.getN() << "\n";
        cout << "2. Number of nodes of the B+ tree: " << BTree.numNodes() << "\n";
        cout << "3. Number of levels of the B+ tree: " << BTree.height(nullptr) << "\n";
        cout << "4. Content of the root node (only the keys): ";
        BTree.display();
        cout << "\n";

        newfile.close();    //close the file object.
    }
}
