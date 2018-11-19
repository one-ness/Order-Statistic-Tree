#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <set>
#include <numeric>
#include "ost.h"
#include "dict.h"
using namespace std;


// CountInversions counts all elements where i<j
// and a[j] < a[i] in the passed set of distinct
// values
int CountInversions(vector<int>& A) {
    OST* t = new OST();
    // origin rank is the original rank in the set
    // value is the value in the set
    // inversions keeps track of total inversion count
    int originRank = 0, value = 0, inversions = 0;
    for(int i = 0; i < A.size(); i++) {
        // rank is the index + 1
        originRank = i + 1;
        value = A.at(i);
        // insert value into tree
        t->Insert(value);
        // increase inversions by difference in ranks
        inversions += originRank - t->Rank(value);
    }
    // free allocated memory
    delete t;
    return inversions;
}


///////////////////////////////////////// do not change anything below for submission ////////////////
void genData(vector<int>& A, int n) {
    A.resize(n);
    set<int> S;
    while (S.size() < n)
        S.insert(rand() % 5000);
    int i = 0;
    for (set<int>::iterator it = S.begin(); it != S.end(); it++)
        A[i++] = *it;
    for (i = 0; i < n; i++) {
        swap(A[i], A[i+ rand()%(n-i)]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Format: " << argv[0] << " [# test cases]\n";
        return -1;
    }
    int cases = atoi(argv[1]);
    vector<int> passed(2, 0);
    for (int c = 1; c <= cases; c++) {
        vector<int> A;
        int id = 0;
        int n = min(1000, 50 + rand()%1001);
        cout << "Case #" << c << "...\n";
        genData(A, n);
        bool ok = true;
        int x = CountInversions(A);
        int y = 0;
        for (int i = 0; i < n; i++)
            for (int j = i+1; j < n; j++)
                y += (A[i] > A[j]);
        ok &= (x == y);
        passed[id++] += ok;
        ok = true;
        DICT D;
        for (int i = 0; i < A.size(); i++)
            D.Insert(A[i]);
        int pos = rand() % A.size();
        int tmp = A[pos];
        while (pos+1 < A.size()) {
            A[pos] = A[pos+1];
            pos++;
        }
        D.Erase(tmp);
        A.resize(A.size()-1);
        sort(A.begin(), A.end());
        int v = A[rand()%A.size()] + 10;
        int avg = D.Average(v), med = D.Median(v);
        vector<int>::iterator it = upper_bound(A.begin(), A.end(), v);
        int num = it - A.begin();
        int avg2 = accumulate(A.begin(), it, 0)/num, med2 = A[(num+1)/2 - 1];
        ok &= (avg == avg2 && med == med2);
        passed[id] += ok;
    }
    cout << "======================= Summary of Testing ============================\n";
    cout << "Problem #1: " << passed[0] << "/" << cases << " passed...\n";
    cout << "Problem #2: " << passed[1] << "/" << cases << " passed...\n";

    return 0;
}

