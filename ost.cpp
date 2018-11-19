// Keegan Fisher
// Algorithms 4100
// PA3 - ost.cpp
#include <cassert>
#include "ost.h"
using namespace std;

void OST::destroy(Node* p) {
    if (!p)
        return;
    destroy(p->left_);
    destroy(p->right_);
    delete p;
}

OST::~OST() {
    destroy(root_);
}

void OST::Insert(int val) {
    Node* parent = NULL;
    Node* cur = root_;
    while (cur && cur->data_ != val) {
        parent = cur;
        // increase each node size by 1
        cur->size_++;
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
    if (!parent)
        root_ = p;
    else if (p->data_ < parent->data_)
        parent->left_ = p;
    else
        parent->right_ = p;
}

int OST::MinElement() const {
    assert(!Empty());
    Node* p = root_;
    while (p->left_)
        p = p->left_;
    return p->data_;
}

int OST::MaxElement() const {
    assert(!Empty());
    Node* p = root_;
    while (p->right_)
        p = p->right_;
    return p->data_;
}

bool OST::Search(int val) const {
    Node* cur = root_;
    while (cur && cur->data_ != val) {
        if (val < cur->data_)
            cur = cur->left_;
        else
            cur = cur->right_;
    }
    return cur != NULL;
}

bool OST::RecurSearch(int val) const {
    return RecurHelper(root_, val);
}

bool OST::RecurHelper(Node* p, int val) const {
    if (!p)
        return false;
    if (p->data_ == val)
        return true;
    if (val < p->data_)
        return RecurHelper(p->left_, val);
    return RecurHelper(p->right_, val);
}

void OST::Erase(int val) {
    Node* parent = NULL;
    Node* cur = root_;
    while (cur && cur->data_ != val) {
        parent = cur;
        // decrease each node size by 1
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
        while (p->left_) { //find successor
            parent = p;
            p = p->left_;
        }
        cur->data_ = p->data_;
        cur = p;
    }
    Node* subtree = cur->left_;
    if (!subtree)
        subtree = cur->right_;
    if (!parent)
        root_ = subtree; //corner case
    else {
        if (parent->left_ == cur)
            parent->left_ = subtree;
        else
            parent->right_ = subtree;
    }
    delete cur;
}

// Select helper that takes extra root node param.
// returns the i-th smallest int in the tree
// algorithm returns -1 if not found
int OST::Select(Node* root, int i) const{
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
int OST::Select(int i) const {
    return Select(this->root_, i);
}

// Rank helper function
// extra parameter is the root of the tree
int OST::Rank(Node* root, int val) const{
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
int OST::Rank(int val) const {
    return Rank(this->root_, val);
}