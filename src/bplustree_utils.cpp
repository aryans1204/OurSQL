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

#include "definitions.hpp"
#include <iostream>
#include <unordered_set>
#include <btree.hpp>
#include "record.hpp"
#include <algorithm>

std::vector<Record::Record*> BTree::BTree::queryRecord(uint key) {
    BNode* temp = this->root;
    while (!temp->isLeaf) {
        if (temp == this->root) {
            temp = root->keys[0] < key ? root->children[1] : root->children[0];
        }
        else {
            int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
            if (key == temp->keys[ind]) temp = temp->children[ind+1];
            else temp = temp->children[ind];
        }
    }
    std::vector<Record::Record*> a;
    if (temp->record.find(key) == temp->record.end()) return a;
    else return temp->record[key];
}

std::vector<Record::Record*> BTree::BTree::queryRecord(uint lower, uint upper) {
    BNode* tempL = this->root;
    while (!tempL->isLeaf) {
        if (tempL == this->root) {
            tempL = root->keys[0] < lower ? root->children[1] : root->children[0];
        }
        else {
            int ind2 = std::upper_bound(tempL->keys.begin(), tempL->keys.end(), lower) - tempL->keys.begin();
            if (lower == tempL->keys[ind2]) tempL = tempL->children[ind2+1];
            else tempL = tempL->children[ind2];        
        }
    }
    //tempL and tempR are both leaf nodes where the lower and upper keys may exist
    std::vector<Record::Record*> ans;
    int i = 0;
    std::vector<Record::Record*> c;
    while (tempL && tempL->keys[i] <= upper) {
        if (tempL->keys[i] < lower) {
            i++;
            continue;
        }
        c = tempL->record[tempL->keys[i++]];
        ans.insert(ans.end(), c.begin(), c.end());
        if (i >= tempL->keys.size()) {
            tempL = tempL->children[i];
            i = 0;
        }
    }
    return ans;
}





