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
#include <set>
#include <iostream>
#include <btree.hpp>
#include "record.hpp"
#include <algorithm>
#include <cmath>


BTree::BTree::BTree(uint n) {
    this->n = n;
    root = nullptr;
    nodes = 0;
}

std::vector<Record::Record> BTree::BTree::queryRecord(uint key) {
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
    std::vector<Record::Record> a;
    if (temp->record.find(key) == temp->record.end()) return a;
    else return temp->record[key];
}

std::vector<Record::Record> BTree::BTree::queryRecord(uint lower, uint upper) {
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
    std::vector<Record::Record> ans;
    int i = 0;
    std::vector<Record::Record> c;
    while (tempL && tempL->keys[i] <= upper) {
        if (tempL->keys[i] < lower) {
            i++;
            continue;
        }
        c = tempL->record[tempL->keys[i++]];
        ans.insert(ans.begin(), c.begin(), c.end());
        if (i >= tempL->keys.size()) {
            tempL = tempL->children[i];
            i = 0;
        }
    }
    return ans;
}

bool BTree::BTree::insertRecord(Record::Record record, uint key) {
    if (root == nullptr) {
        root = new BNode(this->n);
        root->keys.push_back(key);
        root->isLeaf = true;
        std::vector<Record::Record> a;
        root->record[key] = a;
        root->record[key].push_back(record);
        return true;
    }
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
    temp->keys.push_back(key);
    std::sort(temp->keys.begin(), temp->keys.end());
    if (temp->record.find(key) == temp->record.end()) {
        std::vector<Record::Record> a;
        temp->record[key] = a;
    }
    temp->record[key].push_back(record);
    this->balanceTree(true, temp);
    return true;
}

void BTree::BTree::balanceTree(bool leaf, BNode* temp) {
    if (temp->keys.size() > this->n) {
        nodes++;
        int part;
        if (leaf) part = ceil((this->n+1)/2);
        else part = ceil(this->n/2);
        BNode* left = new BNode(this->n);
        left->isLeaf = leaf;
        BNode* right = new BNode(this->n);
        right->isLeaf = leaf;
        left->keys.insert(left->keys.begin(), temp->keys.begin(), temp->keys.begin()+part);
        right->keys.insert(right->keys.begin(), temp->keys.begin()+part, temp->keys.end());
        left->children.insert(left->children.begin(), temp->children.begin(), temp->children.begin()+part);
        right->children.insert(right->children.begin(), right->children.begin()+part, right->children.end());
        if (leaf) {
            for (auto k : left->keys) {
                left->record[k] = temp->record[k];
            }
            for (auto k : right->keys) {
                right->record[k] = temp->record[k];
            }
            left->children[left->children.size()-1] = right;
            right->children[0] = left;
            left->children[0] = temp->children[0];
            right->children[right->children.size()-1] = temp->children[temp->children.size()-1];
        }
        else {
            for (auto p : left->children) {
                p->parent = left;
            }
            for (auto p : right->children) {
                p->parent = right;
            }
        }
    
        BNode* sub = new BNode(this->n);
        sub->keys.push_back(right->keys[0]);
        if (temp->parent == nullptr) {
            nodes++;
            root = sub;
            left->parent = root;
            right->parent = root;
            root->children[0] = left;
            root->children[1] = right;
            delete temp;
            delete sub;
            return;
        }
         
        delete sub;
        uint cand = right->keys[0];
        right->parent = temp->parent;
        left->parent = temp->parent;
        if (temp != root) {
            temp = temp->parent;
            temp->keys.push_back(cand);
            std::sort(temp->keys.begin(), temp->keys.end());
            balanceTree(false, temp);
        }
        delete temp;

    }
    else {
        return;
    }    
}

void BTree::BTree::display() {
    std::vector<uint> a = this->root->keys;
    for (auto k : a) {
        std::cout << "||" << " " << k << " " << "||->";
    }
    std::cout << std::endl;
}

int BTree::BTree::height(BNode* l) {
    int ans = 0;
    if (l == nullptr) l = this->root;
    if (l->isLeaf) return 1;
    for (auto p : l->children) {
        ans = max(ans, height(p));
    }
    return ans+1;
    
}

int BTree::BTree::numNodes() {
    return this->nodes;
}



