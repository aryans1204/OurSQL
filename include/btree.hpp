
#ifndef BTREE_HEADER
#define BTREE_HEADER

#include <vector>
#include <map>
#include <unordered_set>
#include "record.hpp"
#include "definitions.hpp"
namespace BTree {

class BNode {
  public:
    int n=10;  //size of the BNode
    bool isLeaf; //whether node is leaf node
    std::vector<uint> keys; //vector of keys, should be of size n
    std::vector<BNode*> children; //vector of children pointers, of size n+1
    BNode* parent;  //parent BNode
    std::map<uint, std::vector<Record::Record>> record; //map for key to record, only relevant for leaf

    BNode(int n) {
      this->n = n;
      this->parent = nullptr;
      this->isLeaf = false;
      keys(n, 0);
      children(n+1, nullptr);
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
    void balanceTree(bool leaf, BNode* temp);  //function to balanceTree after insertion and deletion, upddates the root accordingly

  public:
    BTree(int n); //constructor for BTree
    bool insertRecord(Record::Record record, uint key); //insert a new record and return it
    bool deleteRecord(uint key); //delete a record based on its key, returns True if sucessful delete
    std::vector<Record::Record> queryRecord(uint key); //return back a record based on its key
    std::vector<Record::Record> queryRecord(uint lower, uint upper);  //overloaded queryRecord with range based queries
    void display(); //prints the current state of the B+tree root node
    int height(BNode* l); //return height of the current tree
    int numNodes();
    ~BTree(); //destructor;  
};
}
#endif