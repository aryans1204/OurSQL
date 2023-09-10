#include <vector>
#include <map>
#include <include/record.hpp>

using namespace BTree;

class BNode {
  uint n; //size of node
  bool isLeaf; //whether a leaf node or not
  BNode* nextNode; //not set to nullptr only when the Node is a leaf node
  std::map<uint, BNode*> ptrTo; //map storing the pointer to the next level for a given key val
  std::map<uint, Record::Record*> record; //map storing pointer to the actual record in the buffer based on key, for leaf nodes.
  BNode(uint n, bool isLeaf); //BNode constructor

};

class BTree {

  private:
    BNode* root; //root of the BTree
    uint n;

  public:
    BTree(uint n); //constructor for BTree
    uint insertRecord(uint teamID, short uint pts, float fg_pct, float ft_pct, float fg3_pct, short uint ast, short uint reb, bool win); //insert a new record and return it
    bool deleteRecord(uint key); //delete a record based on its key, returns True if sucessful delete
    Record::Record* queryRecord(uint key); //return back a record based on its key
    bool updateRecord(uint key, Record::Record* newRec); //update an existing record based on newRec value
    ~BTree(); //destructor;  
};

