#ifndef _OST_H_
#define _OST_H_
#include <iostream>
using namespace std;

class OST {
public:
    class Node {
    public:
        int  data_;
        Node* left_;
        Node* right_;
        //Additional information maintained
        int size_;
    };
    Node* root_;  //root node pointer
    void destroy(Node* p);
    bool RecurHelper(Node* p, int val) const;
    int Select(Node* root, int i) const;
    int Rank(Node* root, int val) const;

public:
    OST() {root_ = NULL; }
    ~OST();
    bool Empty() const { return !root_; }
    void Insert(int val);
    int MinElement() const;
    int MaxElement() const;
    bool Search(int val) const;
    bool RecurSearch(int val) const;
    void Erase(int val);
    int Select(int i) const;
    int Rank(int val) const;
};
#endif