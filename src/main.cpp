#include "buffer_pool.hpp"
#include "btree.hpp"
#include "record.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <set>
#include <tuple>
#include <numeric>
#include <time.h>
#include <algorithm>
#include <cmath>

using namespace BufferPool;
using namespace Record;
using namespace std;

int main() {
    clock_t t;

    BufPool bufPool(104857600, 400);    //pool size = 100MB, block size = 400B

    BTree::BTree BTree(48);

    fstream newfile;

    vector<tuple<void *, uint>> dataset;
    vector<Record::Record> memVector;
    cout << "test";

    cout << "<------------------- Data file read started ------------------->" << "\n" << "\n";

    newfile.open("games.txt",ios::in);  //open a file to perform read operation using file object
    if (newfile.is_open()){     //checking whether the file is open
        string tp;
        int count = 0;
        getline(newfile, tp);   //ignore first line
        set<float> s;
        while(getline(newfile, tp) /*&& count++ < 5*/){    //read data from file object and put it into string.
            //cout << tp << "\n";

            Record::Record record;
            string tempLine;

            stringstream linestream(tp);
            getline(linestream, tempLine,'\t');
            record.date = tempLine;
            linestream >> record.teamID >> record.pts >> record.fg_pct >> record.ft_pct >> record.fg3_pct >> record.ast >> record.reb >> record.win;
            tuple<void *, uint> dataRecord = bufPool.writeRecord(sizeof(record));
            dataset.push_back(dataRecord);

            void *rcdAdr = (uchar *)get<0>(dataRecord) + get<1>(dataRecord);
            memcpy(rcdAdr, &record, sizeof(record));

            memVector.push_back(record);

            //cout << rcdAdr << " " << record.pts << '\n';
            BTree.insertRecord(record, record.fg_pct);
            //cout << rcdAdr << " " << record.fg_pct << '\n';
            //BTree.display();
            s.insert(record.fg_pct);
        }
        std::sort (memVector.begin(), memVector.end(), [](const Record::Record& lhs, const Record::Record& rhs) {
            return lhs.fg_pct < rhs.fg_pct;
        });
        cout << "<------------------- Data file read ended ------------------->" << "\n" << "\n";

        cout << "<------------------- Storage Statistics (Experiment 1) ------------------->" << "\n";
        cout << "1. Number of records: " << bufPool.getNumRecords() << "\n";
        cout << "2. Size of a record: " << sizeof(Record::Record) << "\n";
        cout << "3. Number of records stored in a block: " << bufPool.getBlkSize()/sizeof(Record::Record) << "\n";
        cout << "4. Number of allocated blocks: " << bufPool.getNumAllocBlks() << "\n\n";

        cout << "<------------------- B+ Tree Statistics (Experiment 2) ------------------->" << "\n";
        cout << "1. Parameter n of B+ tree: " << 48 << "\n";
        cout << "2. Number of nodes of the B+ tree: " << BTree.numNodes() << "\n";
        cout << "3. Number of levels of the B+ tree: " << BTree.height() << "\n";
        cout << "4. Content of the root node (only the keys): ";
        BTree.display();
        cout << "\n";

        cout << "<------------------- B+ Tree Querying (Experiment 3) ------------------->" << "\n";
        int blks = 0;
        t = clock();
        auto a = BTree.queryRecord(0.5, blks);
        t = clock() - t;
        cout << "Number of index blocks accessed: " << blks << endl;
        cout << "Number of data blocks accessed: " << a.size() / (bufPool.getBlkSize()/sizeof(Record::Record)) << endl;
        cout << "Average of FG3_PCT_HOME: ";
        float sum = 0;
        for (auto r : a) {
            sum += r.fg3_pct;
        }
        cout << sum/a.size() << endl;
        cout << "sum: " << sum << ", count: " << a.size() << endl;
        cout << "Running time for query: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;

        
        //Using brute force
        cout << "\n<----Using brute-force method---->" << endl;
        sum = 0;
        count = 0;
        int index = 1;
        t = clock();
        for (int i = 0; i < memVector.size(); i++) {
            if (memVector[i].fg_pct == 0.5) {
                sum += memVector[i].fg3_pct;
                count++;
            }
            if (memVector[i].fg_pct > 0.5) {
                break;
            }
            index++;
        }
        t = clock() - t;
        cout << "Number of data blocks accessed: " << ceil(index/6) << endl;
        cout << "Average of FG3_PCT_HOME: " << sum/count << endl;
        cout << "sum: " << sum << ", count: " << count << endl;
        cout << "Running time for query: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;
        cout << "\n";

        cout << "<------------------- B+ Tree Range Based Querying (Experiment 4) ------------------->" << "\n";
        blks = 0;
        t = clock();
        a = BTree.queryRecord(0.6, 1, blks);
        t = clock() - t;
        cout << "Number of index blocks accessed: " << blks << endl;
        cout << "Number of data blocks accessed: " << a.size() / (bufPool.getBlkSize()/sizeof(Record::Record)) << endl;
        cout << "Average of FG3_PCT_HOME: ";
        sum = 0;
        for (auto r : a) {
            sum += r.fg3_pct;
        }
        cout << sum/a.size() << endl;
        cout << "sum: " << sum << ", count: " << a.size() << endl;
        cout << "Running time for query: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;

        
        //Using brute force
        cout << "\n<----Using brute-force method---->" << endl;
        sum = 0;
        count = 0;
        index = 1;
        t = clock();
        for (int i = 0; i < memVector.size(); i++) {
            if (memVector[i].fg_pct >= 0.6 && memVector[i].fg_pct <= 1) {
                sum += memVector[i].fg3_pct;
                count++;
            }
            if (memVector[i].fg_pct > 1) {
                break;
            }
            index++;
        }
        t = clock() - t;
        cout << "Number of data blocks accessed: " << ceil(index/6) << endl;
        cout << "Average of FG3_PCT_HOME: " << sum/count << endl;
        cout << "sum: " << sum << ", count: " << count << endl;
        cout << "Running time for query: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;

        cout << "<------------------- B+ Tree Deleting Records (Experiment 5) ------------------->" << "\n";
        a = BTree.queryRecord(0.35, blks);
        cout << a.size() << endl;

        t = clock();
        //a = BTree.queryRecord(0, 0.35, blks);
        for (int i = 0; i < memVector.size(); i++) {
            if (memVector[i].fg_pct <= 0.35 && s.find(memVector[i].fg_pct) != s.end()) {
                BTree.deleteRecord(memVector[i].fg_pct);
                s.erase(memVector[i].fg_pct);
            }
        }
        /*for (int i = 0; i < a.size(); i++) {
            BTree.deleteRecord(a[i].fg_pct);
        }*/
        t = clock() - t;

        //a = BTree.queryRecord(0.35, blks);
        //cout << a .size() << endl;

        cout << "Number of nodes in updated B+ tree: " << BTree.numNodes() << endl;
        cout << "Number of levels in updated B+ tree: " << BTree.height() << endl;
        cout << "Content of the root node (only the keys): ";
        BTree.display();
        cout << "Running time of record deletions: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;

        //Using brute force
        cout << "\n<----Using brute-force method---->" << endl;
        sum = 0;
        count = 0;
        index = 1;
        t = clock();
        for (int i = 0; i < memVector.size(); i++) {
            if (memVector[i].fg_pct <= 0.35) {
                memVector.erase(memVector.begin() + i);
            }
            if (memVector[i].fg_pct > 0.35) {
                break;
            }
            index++;
        }
        t = clock() - t;

        cout << "Number of data blocks accessed: " << ceil(index/6) << endl;
        cout << "Running time of record deletions: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;

        cout << "Number of nodes in updated B+ tree: " << BTree.numNodes() << endl;
        cout << "Number of levels in updated B+ tree: " << BTree.height() << endl;
        cout << "Content of the root node (only the keys): ";
        BTree.display();
        cout << "Running time of record deletions: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;

        //Using brute force
        cout << "\n<----Using brute-force method---->" << endl;
        sum = 0;
        count = 0;
        index = 1;
        t = clock();
        for (int i = 0; i < memVector.size(); i++) {
            if (memVector[i].fg_pct <= 0.35) {
                memVector.erase(memVector.begin() + i);
            }
            if (memVector[i].fg_pct > 0.35) {
                break;
            }
            index++;
        }
        t = clock() - t;

        cout << "Number of data blocks accessed: " << ceil(index/6) << endl;
        cout << "Running time of record deletions: " << t << " clicks" << "(" << ((float) t)/CLOCKS_PER_SEC << "s)" << endl;

        newfile.close();    //close the file object.
    }
}