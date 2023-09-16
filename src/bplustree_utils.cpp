// We consider the 2 Utility functions: (i) Insertion and (ii) Deletion for our B+ Tree below

// (ii) Deletion

/* In deletion, we need to consider 3 scenarios depending on whether the type of node to be deleted is: leaf, internal, or both

Scenario 1: Leaf node
            Sub-scenario 1: deletion procedure doesn't cause disturbance to min no of keys => just delete
            Sub-scenario 2: deletion procesure causes disturbance to min no of keys => delete and get key from neighbor

Scenario 2: Internal node
            Sub-scenario 1: Delete and get key from left full (meaning having keys > min) child
            Sub-scenario 2: Delere and get key from right full child
            Sub-scenario 3: Delete and merge children if both have same no of min keys

Scenario 3: Upon deletion of an internal node, min no of keys not satisfied => merge the neighbor
*/

#include <iostream>
#include <btree.hpp>
#include "record.hpp"
#include "definitions.hpp"
#include <algorithm>

Record::Record* BTree::BTree::queryRecord(uint key) {
    BNode* temp = this->root;
    while (!temp->isLeaf) {
        if (temp == this->root) {
            temp = root->keys[0] < key ? root->children[0] ? root->children[1];
        }
        else {
            int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
            if (key == temp->keys[ind]) temp = temp->children[ind+1];
            else temp = temp->children[ind];
        }
    }
    if (temp->record.find(key) == temp->record.end()) return nullptr;
    else return temp->record[key];
}



