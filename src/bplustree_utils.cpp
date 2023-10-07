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
        //std::cout << "WHY" << std::endl;
        int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
        //std::cout << "IND " << ind << std::endl;
        if (ind >= temp->keys.size()) {
            temp = temp->children[temp->keys.back()];
        }
        else if (ind == 0) temp = temp->lower;
        else if (key == temp->keys[ind]) temp = temp->children[temp->keys[ind]];
        else temp = temp->children[temp->keys[ind-1]];
        //std::cout << "KEy size " << temp->keys.size() << std::endl;
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
    //std::cout << "Start" << std::endl;
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
        if (temp->keys.size() > n) std::cout << "FULLup!!" << std::endl;
        //std::cout << "WHY" << std::endl;
        int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
        //std::cout << "IND " << ind << std::endl;
        if (ind >= temp->keys.size()) {
            temp = temp->children[temp->keys.back()];
        }
        else if (ind == 0) temp = temp->lower;
        else if (key == temp->keys[ind]) temp = temp->children[temp->keys[ind]];
        else temp = temp->children[temp->keys[ind-1]];
        //std::cout << temp->keys.size() << std::endl;
    }
    if (temp->keys.size() > n) std::cout << "FULL!!" << std::endl;
    if (temp != root && temp->keys.size() < (n+1)/2) std::cout << temp->keys.size() << std::endl;
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
    //std::cout << "Here" << std::endl;
    this->balanceTree(true, temp);
    //std::cout << "POst temp" << std::endl;
    return true;
}

std::pair<BTree::BNode*, BTree::BNode*> BTree::BTree::balanceTree(bool leaf, BNode* temp) {
    if (temp->keys.size() > this->n) {
        //cout << "balanceTree (full): ";
        //display();
        nodes++;
        int part;
        if (leaf) part = ceil((double) (this->n+1)/2);
        else part = ceil((double) this->n/2);
        BNode* left = new BNode(this->n);
        left->isLeaf = leaf;
        BNode* right = new BNode(this->n);
        right->isLeaf = leaf;
        left->keys.insert(left->keys.begin(), temp->keys.begin(), temp->keys.begin()+part);
        //std::cout << "Left: " << left->keys.size() << std::endl;
        right->keys.insert(right->keys.begin(), temp->keys.begin()+part, temp->keys.end());
        //std::cout << "Right: " << right->keys.size() << std::endl;
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
            //std::cout << "Done" << std::endl;
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
            //right->lower = left->children[left->keys.back()];
            right->lower = right->children[right->keys[0]];
        }

        BNode* sub = new BNode(this->n);
        sub->keys.push_back(right->keys[0]);
        float kl = right->keys[0];
        if (temp->parent == nullptr) {
            //std::cout << "temp parent" << std::endl;
            BNode* al = root;
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
            if (!leaf) right->keys.erase(right->keys.begin());
            if (!leaf) right->children.erase(cand);
            if (temp->parent->lower == temp) temp->lower = left;
            for (auto b : temp->parent->children) {
                if (b.second == temp) {
                    temp->parent->children[b.first] = left;
                    break;
                }
            }
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
        delete temp;
    }
    else {
        return make_pair(temp, temp);
    }    
}
bool BTree::BTree::deleteRecord(float key) {
    BNode* temp = this->root;
    while (!temp->isLeaf) {
        int ind = std::upper_bound(temp->keys.begin(), temp->keys.end(), key) - temp->keys.begin();
        if (ind >= temp->keys.size()) {
            temp = temp->children[temp->keys.back()];
        }
        else if (ind == 0) temp = temp->lower;
        else if (key == temp->keys[ind]) temp = temp->children[temp->keys[ind]];
        else temp = temp->children[temp->keys[ind-1]];
        if (temp == nullptr) std::cout << "NULL" << std::endl;
    }
    auto ing = std::find(temp->keys.begin(), temp->keys.end(), key);
    temp->keys.erase(ing);
    temp->children.erase(key);
    temp->record.erase(key);
    //std::cout << "Pre del" << std::endl;
    balanceDel(true, temp, key, ing-temp->keys.begin());
    return true;
}

void BTree::BTree::balanceDel(bool leaf, BNode* temp, float ks, int ind) {
    if (leaf) {
        if (temp->keys.size() >= (n+1)/2) {
            if (ind != 0) return;
            float cl = temp->keys[0];
            BNode* p = temp->parent;
            if (std::find(p->keys.begin(), p->keys.end(), ks) != p->keys.end()) {
                p->children[cl] = p->children[ks];
                p->children.erase(ks);
            }
            std::replace(p->keys.begin(), p->keys.end(), ks, cl);
            while (p != root) {
                p = p->parent;
                if (std::find(p->keys.begin(), p->keys.end(), ks) != p->keys.end()) {
                    p->children[cl] = p->children[ks];
                    p->children.erase(ks);
                }
                std::replace(p->keys.begin(), p->keys.end(), ks, cl);
            }
            return;
        }
        BNode* left = temp->children[1];
        BNode* right = temp->children[2];
        if (left != nullptr && left->keys.size() >= (n+1)/2+1) {
            temp->keys.insert(temp->keys.begin(), left->keys.end()-1, left->keys.end());
            temp->record[left->keys[left->keys.size()-1]] = left->record[left->keys.size()-1];
            float k = left->keys[left->keys.size()-1];
            left->keys.erase(left->keys.end()-1);
            left->record.erase(k);
            BNode* p = temp->parent;
            if (std::find(p->keys.begin(), p->keys.end(), ks) != p->keys.end()) {
                p->children[k] = p->children[ks];
                p->children.erase(ks);
            }
            std::replace(p->keys.begin(), p->keys.end(), ks, k);
            while (p != root) {
                p = p->parent;
                if (std::find(p->keys.begin(), p->keys.end(), ks) != p->keys.end()) {
                    p->children[k] = p->children[ks];
                    p->children.erase(ks);
                }
            }
                std::replace(p->keys.begin(), p->keys.end(), ks, k);
            return;
        }
        else if (right != nullptr && right->keys.size() >= (n+1)/2+1) {
            float k = temp->keys[0];
            BNode* p = temp->parent;
            if (std::find(p->keys.begin(), p->keys.end(), ks) != p->keys.end()) {
                p->children[k] = p->children[ks];
                p->children.erase(ks);
            }
            std::replace(p->keys.begin(), p->keys.end(), ks, k);
            while (p != root) {
                p = p->parent;
                if (std::find(p->keys.begin(), p->keys.end(), ks) != p->keys.end()) {
                    p->children[k] = p->children[ks];
                    p->children.erase(ks);
                }
            }
            std::replace(p->keys.begin(), p->keys.end(), ks, k);
            temp->keys.push_back(right->keys[0]);
            k = right->keys[0];
            temp->record[k] = right->record[k];
            right->keys.erase(right->keys.begin());
            right->record.erase(k);
            p = temp->parent;
            std::replace(p->keys.begin(), p->keys.end(), k, right->keys[0]);
            while (p != root) {
                p = p->parent;
                std::replace(p->keys.begin(), p->keys.end(), k, right->keys[0]);
            }
            return;
        }
        else if (left != nullptr) {
            nodes--;
            left->keys.insert(left->keys.end()-1, temp->keys.begin(), temp->keys.end());
            for (auto p : temp->keys) {
                left->record[p] = temp->record[p];
            }
            left->children[2] = right;
            BNode* p = temp->parent;
            for (auto b : p->children) {
                if (b.second == temp) {
                    p->keys.erase(std::find(p->keys.begin(), p->keys.end(), b.first));
                    p->children.erase(b.first);
                    break;
                }
            }
            delete temp;
            return;
        }
        else {
            nodes--;
            right->keys.insert(right->keys.begin(), temp->keys.begin(), temp->keys.end());
            for (auto p : temp->keys) {
                right->record[p] = temp->record[p];
            }
            right->children[1] = left;
            BNode* p = temp->parent;
            p->lower = right;
            p->children.erase(p->keys[0]);
            p->keys.erase(p->keys.begin());
            delete temp;
            return;
        }
    }   
    else {
        if (temp->keys.size() >= n/2) return;
    } 
}
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



