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
#include <utility>
#include <iostream>
#include <btree.hpp>
#include "record.hpp"
#include <algorithm>
#include <cmath>


BTree::BTree::BTree(int n) {
    this->n = n;
    root = nullptr;
    nodes = 0;
}

BTree::BTree::~BTree() {
    BNode* temp = root;
}

std::vector<Record::Record> BTree::BTree::queryRecord(float key, int& blks) {
    BNode* temp = this->root;
    blks++;
    while (!temp->isLeaf) {
        std::cout << "WHY" << std::endl;
        int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
        std::cout << "IND " << ind << std::endl;
        if (ind >= temp->keys.size()) {
            temp = temp->children[temp->keys.back()];
        }
        else if (ind == 0) temp = temp->lower;
        else if (key == temp->keys[ind]) temp = temp->children[temp->keys[ind]];
        else temp = temp->children[temp->keys[ind-1]];
        std::cout << "KEy size " << temp->keys.size() << std::endl;
        blks++;
    }
    std::vector<Record::Record> a;
    if (temp->record.find(key) == temp->record.end()) return a;
    else return temp->record[key];
}

std::vector<Record::Record> BTree::BTree::queryRecord(float lower, float upper, int& blks) {
    BNode* tempL = this->root;
    blks++;
    while (!tempL->isLeaf) {
        int ind = std::upper_bound(tempL->keys.begin(), tempL->keys.end(), lower) - tempL->keys.begin();
        if (ind >= tempL->keys.size()) {
            tempL = tempL->children[tempL->keys.back()];
        }
        else if (ind == 0) tempL = tempL->lower;
        else if (lower == tempL->keys[ind]) tempL = tempL->children[tempL->keys[ind]];
        else tempL = tempL->children[tempL->keys[ind-1]];
        blks++;
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
            blks++;
            tempL = tempL->children[2];
            i = 0;
        }
    }
    return ans;
}

bool BTree::BTree::insertRecord(Record::Record record, float key) {
    std::cout << "Start" << std::endl;
    if (root == nullptr) {
        h = 1;
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
        std::cout << "WHY" << std::endl;
        int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
        std::cout << "IND " << ind << std::endl;
        if (ind >= temp->keys.size()) {
            temp = temp->children[temp->keys.back()];
        }
        else if (ind == 0) temp = temp->lower;
        else if (key == temp->keys[ind]) temp = temp->children[temp->keys[ind]];
        else temp = temp->children[temp->keys[ind-1]];
        std::cout << temp->keys.size() << std::endl;
    }
    if (temp->record.find(key) != temp->record.end()) {
        temp->record[key].push_back(record);
        return true;
    }
    temp->keys.push_back(key);
    std::sort(temp->keys.begin(), temp->keys.end());
    if (temp->record.find(key) == temp->record.end()) {
        std::vector<Record::Record> a;
        temp->record[key] = a;
    }
    temp->record[key].push_back(record);
    std::cout << "Here" << std::endl;
    this->balanceTree(true, temp);
    std::cout << "POst temp" << std::endl;
    return true;
}

std::pair<BTree::BNode*, BTree::BNode*> BTree::BTree::balanceTree(bool leaf, BNode* temp) {
    if (temp->keys.size() > this->n) {
        nodes++;
        int part;
        if (leaf) part = ceil((double) (this->n+1)/2);
        else part = ceil(this->n/2);
        BNode* left = new BNode(this->n);
        left->isLeaf = leaf;
        BNode* right = new BNode(this->n);
        right->isLeaf = leaf;
        left->keys.insert(left->keys.begin(), temp->keys.begin(), temp->keys.begin()+part);
        std::cout << left->keys.back() << std::endl;
        right->keys.insert(right->keys.begin(), temp->keys.begin()+part, temp->keys.end());
        std::cout << right->keys[0] << std::endl;
        if (leaf) {
            for (auto k : left->keys) {
                left->record[k] = temp->record[k];
            }
            for (auto k : right->keys) {
                right->record[k] = temp->record[k];
            }
            left->children[2] = right;
            right->children[1] = left;
            left->children[1] = temp->children[1];
            right->children[2] = temp->children[2];
            std::cout << "Done" << std::endl;
        }
        else {
            for (auto p : left->keys) {
                left->children[p] = temp->children[p];
            }
            for (auto p : right->keys) {
                right->children[p] = temp->children[p];
            }
            for (auto p : left->children) {
                p.second->parent = left;
            }
            for (auto p : right->children) {
                p.second->parent = right;
            }
            left->lower = temp->lower;
            right->lower = left->children[left->keys.back()];
        }
    
        BNode* sub = new BNode(this->n);
        sub->keys.push_back(right->keys[0]);
        float kl = right->keys[0];
        if (temp->parent == nullptr) {
            std::cout << "temp parent" << std::endl;
            if (!leaf) right->children.erase(right->keys[0]);
            if (!leaf) right->keys.erase(right->keys.begin());
            nodes++;
            h++;
            root = sub;
            left->parent = root;
            right->parent = root;
            root->lower = left;
            root->children[kl] = right;
            //delete temp;
            return make_pair(left, right);
        }
         
        else delete sub;
        float cand = right->keys[0];
        //right->parent = temp->parent;
        //left->parent = temp->parent;
        if (temp != root) {
            temp = temp->parent;
            temp->children[cand] = right;
            if (cand < temp->keys[0]) temp->lower = left;
            temp->keys.push_back(cand);
            std::sort(temp->keys.begin(), temp->keys.end());
            auto l = balanceTree(false, temp);
            left->parent = l.first;
            right->parent = l.second;
            return make_pair(left, right);
        }
        //delete temp;

    }
    else {
        return make_pair(temp, temp);
    }    
}
/*bool BTree::BTree::deleteRecord(float key) {
    BNode* temp = this->root;
    while (!temp->isLeaf) {
        int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
        if (key == temp->keys[ind]) temp = temp->children[ind+1];
        else temp = temp->children[ind];
    }
    temp->keys.erase(temp->keys.begin(), key);
    temp->children.erase(key);
    temp->record.erase(key);
    balanceDel(true, temp, key);
    return true;
}

void BTree::BTree::balanceDel(bool leaf, BNode* temp, float ks) {
    if (leaf) {
        if (temp->keys.size() >= (n+1)/2) return;
        BNode* left = temp->children[1];
        BNode* right = temp->children[2];
        if (left != nullptr && left->keys.size() >= (n+1)/2+1) {
            temp->insert(temp->keys.begin(), left->keys.end()-1, left->keys.end());
            temp->record[left->keys[left->keys.size()-1]] = left->record[left->keys.size()-1];
            float k = left->keys[left->keys.size()-1];
            left->keys.erase(left->begin(), k);
            left->record.erase(k);
            BNode* p = temp->parent;
            auto it = std::find(p->keys.begin(), p->keys.end(), ks);
            while (p != nullptr && it != p->keys.end()) {
                p = p->parent;
                if (p != nullptr) it = std::find(p->keys.begin(), p->keys.end(), ks);
            }
            if (p != nullptr) p[it-p->keys.begin()] = temp->keys[0];
            return;
        }
        else if (right != nullptr && right->keys.size() >= (n+1)/2+1) {
            temp->insert(temp->keys.end()-1, right->keys.begin(), right->keys.begin()+1);
            float k = right->keys[0];
            temp->record[k] = right->record[k];
            right->keys.erase(right->keys.begin(), k);
            right->record.erase(k);
            BNode* p = temp->parent;
            auto it = std::find(p->keys.begin(), p->keys.end(), k);
            while (p != nullptr && it != p->keys.end()) {
                p = p->parent;
                if (p != nullptr) it = std::find(p->keys.begin(), p->keys.end(), k);
            }
            if (p != nullptr) p[it-p->keys.begin()] = right->keys[0];
            return;
        }
        else if (left != nullptr) {
            left->keys.insert(left->keys.end()-1, temp->keys.begin(), temp->keys.end());
            left->children[2] = right;
            for (auto p : temp->keys) {
                left->record[p] = temp->record[p];
            }
            balanceDel(false, temp->parent, );
            delete temp;
        }
        else {
            float u = right->keys[0];
            right->keys.insert(right->keys.begin(), temp->keys.begin(), temp->keys.end());
            right->children[1] = left;
            for (auto p : temp->keys) {
                right->record[p] = temp->record[p];
            }
            BNode* p = right->parent;
            auto it = std::find(p->keys.begin(), p->keys.end(), u);
            while (p != nullptr && it != p->keys.end()) {
                p = p->parent;
                if (p != nullptr) it = std::find(p->keys.begin(), p->keys.end(), u);
            }
            if (right->parent == temp->parent) {
                p->keys
            }
            balanceDel(false, temp->parent);
            delete temp;
        }
    }   
    else {
        if (temp->keys.size() >= n/2) return;
        else if ()

    } 
}*/
void BTree::BTree::display() {
    for (auto k : this->root->keys) {
        std::cout << "||" << " " << k << " " << "||->";
    }
    std::cout << std::endl;
}

int BTree::BTree::height() {
    return h;   
}

int BTree::BTree::numNodes() {
    return this->nodes;
}



