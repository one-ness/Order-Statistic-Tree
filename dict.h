#ifndef _DICT_H_
#define _DICT_H_
#include <iostream>
using namespace std;

class DICT {
public:
    class Node {
    public:
        int  data_;
        Node* left_;
        Node* right_;
        Node* parent_;
        //Additional information maintained
        int subtreeTotal_;
        int size_;
    };
    Node* root_;  //root node pointer
    void destroy(Node* p);
    bool RecurHelper(Node* p, int val) const;
    int Select(Node* root, int i) const;
    int Average(Node* root, int val, int &count) const;
    int Median(Node* root, int val) const;
    int Rank(Node* root, int val) const;
public:
    DICT() {root_ = NULL; }
    ~DICT();
    bool Empty() const { return !root_; }
    void Insert(int val);
    int MinElement() const;
    int MaxElement() const;
    bool Search(int val) const;
    bool RecurSearch(int val) const;
    void Erase(int val);
    int Average(int val) const;
    int Median(int val) const;
    int Select(int i) const;
    int Rank(int val) const;
};
#endif