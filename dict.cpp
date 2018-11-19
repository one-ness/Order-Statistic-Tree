// Keegan Fisher
// Algorithms 4100
// PA3 - dict.cpp
#include <cassert>
#include "dict.h"

using namespace std;
void DICT::destroy(Node* p) {
    if (!p)
        return;
    destroy(p->left_);
    destroy(p->right_);
    delete p;
}

DICT::~DICT() {
    destroy(root_);
}

void DICT::Insert(int val) {
    Node* parent = NULL;
    Node* cur = root_;
    while (cur && cur->data_ != val) {
        parent = cur;
        // increase node size by 1
        cur->size_++;
        // increase subtreetotal by value added
        cur->subtreeTotal_ += val;
        if (val < cur->data_)
            cur = cur->left_;
        else
            cur = cur->right_;
    }
    if (cur)        return; //found!
    Node* p = new Node;
    if (!p) return;
    p->data_ = val;
    p->left_ = p->right_ = NULL;
    p->size_ = 1;
    p->subtreeTotal_ = val;
    if (!parent) {
        root_ = p;
        root_->parent_ = NULL;
    }
    else if (p->data_ < parent->data_) {
        parent->left_ = p;
        parent->left_->parent_ = parent;
    }
    else {
        parent->right_ = p;
        parent->right_->parent_ = parent;
    }
}

int DICT::MinElement() const {
    assert(!Empty());
    Node* p = root_;
    while (p->left_)
        p = p->left_;
    return p->data_;
}

int DICT::MaxElement() const {
    assert(!Empty());
    Node* p = root_;
    while (p->right_)
        p = p->right_;
    return p->data_;
}

bool DICT::Search(int val) const {
    Node* cur = root_;
    while (cur && cur->data_ != val) {
        if (val < cur->data_)
            cur = cur->left_;
        else
            cur = cur->right_;
    }
    return cur != NULL;
}

bool DICT::RecurSearch(int val) const {
    return RecurHelper(root_, val);
}

bool DICT::RecurHelper(Node* p, int val) const {
    if (!p)
        return false;
    if (p->data_ == val)
        return true;
    if (val < p->data_)
        return RecurHelper(p->left_, val);
    return RecurHelper(p->right_, val);
}

void DICT::Erase(int val) {
    Node* parent = NULL;
    Node* cur = root_;
    while (cur && cur->data_ != val) {
        parent = cur;
        // decrease every node's subtreeTotal by value
        cur->subtreeTotal_ -= val;
        // decrease every node's size by 1
        cur->size_--;
        if (cur->data_ > val)
            cur = cur->left_;
        else
            cur = cur->right_;
    }
    if (!cur)
        return;
    if (cur->left_ && cur->right_) { //case 3
        parent = cur;
        Node* p = cur->right_;
        while (p->left_) { //find succesor
            parent = p;
            p = p->left_;
        }
        // decrease the subtreeTotal by the value removed
        cur->subtreeTotal_ = cur->subtreeTotal_ - cur->data_;
        // reset the data of this node to new parent
        cur->data_ = p->data_;
        // decrease the size by 1 for having removed the node
        cur->size_--;
        cur = p;
    }
    Node* subtree = cur->left_;
    if (!subtree)
        subtree = cur->right_;
    if (!parent)
        root_ = subtree; // edge case
    else {
        if (parent->left_ == cur) {
            parent->left_ = subtree;
            // reset the values from the subtree
            parent->subtreeTotal_ = parent->data_;
            parent->size_ = 1;
            if (parent->left_)
            {
                parent->size_ += parent->left_->size_;
                parent->subtreeTotal_ += parent->left_->subtreeTotal_;
            }
            if (parent->right_)
            {
                parent->size_ += parent->right_->size_;
                parent->subtreeTotal_ += parent->right_->subtreeTotal_;
            }

        }
        else {
            parent->right_ = subtree;
            // update values in the subtree
            parent->subtreeTotal_ = parent->data_;
            parent->size_ = 1;
            if (parent->left_)
            {
                parent->size_ += parent->left_->size_;
                parent->subtreeTotal_ += parent->left_->subtreeTotal_;
            }
            if (parent->right_)
            {
                parent->size_ += parent->right_->size_;
                parent->subtreeTotal_ += parent->right_->subtreeTotal_;
            }
        }

        Node* node = parent;
        while (node)
        {
            // move toward the root the update the values from the subtree
            node->subtreeTotal_ = node->data_;
            node->size_ = 1;
            if (node->left_) {
                node->subtreeTotal_ += node->left_->subtreeTotal_;
                node->size_ += node->left_->size_;

            }
            if (node->right_) {
                node->subtreeTotal_ += node->right_->subtreeTotal_;
                node->size_ += node->right_->size_;
            }

            node = node->parent_;
        }
    }
    delete cur;
}


// Rank helper function
// extra parameter is the root of the tree
int DICT::Rank(Node* root, int val) const{
    if(root) {
        // if found, return the count of the left subtree
        // + 1 for current node
        if(root->data_ == val) {
            if(root->left_) return root->left_->size_ + 1;
            return 1;
        }
        // recurse left
        if(root->data_ > val) return Rank(root->left_, val);
        // recurse right: adding left subtree size + 1 for current node
        if(root->left_) {
            return root->left_->size_ + 1 + Rank(root->right_, val);
        } else {
            return 1 + Rank(root->right_, val);
        }
    }
    return -1;
}

// returns the Rank of the value in the tree
// Rank is the order (how many numbers are below it + 1)
// It returns an int and takes an int val to find
// returns -1 if not found
int DICT::Rank(int val) const {
    return Rank(this->root_, val);
}


int DICT::Average(Node* r, int val, int &count) const {
    if(r) {
        if(r->data_ <= val) {
            if(r->left_) {
                count+= r->left_->size_ + 1;
                return r->left_->subtreeTotal_ + r->data_ + Average(r->right_, val, count);
            }
            return r->data_ + Average(r->right_, val, (count+= 1));
        }
        if(r->data_ > val) return Average(r->left_, val, count);
        if(r->left_) {
            count+= r->left_->size_;
            return r->left_->subtreeTotal_;
        }
        return 0;
    }
    return 0;
}

int DICT::Average(int val) const {
    int count = 0;
    return Average(root_, val, count) / (count);
}


// Returns the average of nodes with value <= val
/*int DICT::Average(int val) const {
    // sum of all data <= val
    int sum = 0;
    // count of all nodes w/ data <= val
    int count = 0;

    Node* cur = root_;
    while (cur) {
        // if curr->data_ <= val, add left subtreeTotal and the sum to
        // to the curr->data_ and traverse right
        if (cur->data_ <= val) {
            if (cur->left_) {
                sum = sum + cur->left_->subtreeTotal_ + cur->data_;
                count = count + cur->left_->size_ + 1;
            }
            else {
                sum = sum + cur->data_;
                count++;
            }
            cur = cur->right_;
        }
        // traverse left
        else
            cur = cur->left_;
    }
    // return the average of values <= val
    return (sum / count);
}*/

// Select helper that takes extra root node param.
// returns the i-th smallest int in the tree
// algorithm returns -1 if not found
int DICT::Select(Node* root, int i) const {
    if(root) {
        int r;
        // increase r by all values less than
        // that are being ignored
        if(root->left_) r = root->left_->size_ + 1;
        else r = 1;
        // found the i-th element
        if(r == i) return root->data_;
        //recurse left, don't decrease rank since
        // all other values are greater
        if(i < r) return Select(root->left_, i);
        // recurse right, decreasing i by r,
        // the values less than it
        return Select(root->right_, i - r);
    }
    return -1;
}

// Select finds the i-th smallest value
// in the tree and returns it's value
int DICT::Select(int i) const {
    return Select(this->root_, i);
}


// Returns the floored median of values less than or equal
// to the passed val
int DICT::Median(int val) const {
    // keep track of nodes with key <= val
    int count = 0;

    Node* curr = root_;
    while (curr) {
        // If the curr->data <= val, increase count
        // by left subtree size + 1 and traverse right
        if (curr->data_ <= val) {
            if (curr->left_) count = count + curr->left_->size_ + 1;
            else count++;
            curr = curr->right_;
        }
        // traverse right
        else curr = curr->left_;
    }

    // use the count to find the i-th smallest element,
    // i being the median index
    return Select((count + 1) / 2);
}

/*
int DICT::Median(int val) const {
    int m = (Rank(val) + 1)/ 2;
    int median = Select(m);
    return median;
}
 */