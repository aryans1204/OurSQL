
#ifndef BTREE_HEADER
#define BTREE_HEADER

#include <vector>
#include <map>
#include <unordered_set>
#include "record.hpp"
#include "definitions.hpp"
#include <utility>
namespace BTree {

class BNode {
  public:
    int n;  //size of the BNode
    bool isLeaf; //whether node is leaf node
    std::vector<float> keys; //vector of keys, should be of size n
    std::map<float, BNode*> children; //vector of children pointers, of size n+1
    BNode* parent;  //parent BNode
    std::map<float, std::vector<Record>> record; //map for key to record, only relevant for leaf
    BNode* lower;

    BNode(int n) {
      this->n = n;
      this->parent = nullptr;
      this->isLeaf = false;
    }

    friend bool operator<(const BNode& a, const BNode& b) {
      return a.keys[0] < b.keys[0];
    }
    friend bool operator<=(const BNode& a, const BNode& b) {
      return a.keys[0] <= b.keys[0];
    }
};

class BTree {

  private:
    int nodes;
    BNode* root; //root of the BTree
    int n;
    std::pair<BNode*, BNode*> balanceTree(bool leaf, BNode* temp);  //function to balanceTree after insertion and deletion, upddates the root accordingly
    int h;

  public:
    BTree(int n); //constructor for BTree
    bool insertRecord(Record record, float key); //insert a new record and return it
    bool deleteRecord(float key); //delete a record based on its key, returns True if sucessful delete
    std::vector<Record::Record> queryRecord(float key, int& blks); //return back a record based on its key
    std::vector<Record::Record> queryRecord(float lower, float upper, int& blks);  //overloaded queryRecord with range based queries
    void display(); //prints the current state of the B+tree root node
    int height(); //return height of the current tree
    int numNodes();
    int getN();
    ~BTree(); //destructor;  
};
}
#endif
