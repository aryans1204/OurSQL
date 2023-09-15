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

# include <iostream>
using namespace std;

bool flag_is_leaf = 0;
bool flag_is_internal = 0;

// write a fucntion getK to get the key of node to be deleted

bool deleteRecord(uint)
{
            int index = getK(key);

            // check if key to be deleted is in present node
            if(karr[index] = key && index < totK)
            {
                 if(bool_is_leaf)       
            }
            
}



