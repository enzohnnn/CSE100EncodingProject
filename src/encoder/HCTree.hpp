#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    /* TODO: Initializes a new empty HCTree.*/
    HCTree() { root = nullptr; }

    ~HCTree();

    void preorderTraversal(HCNode* current, string& str);

    string buildString();

    HCNode* rebuildRecursively(istream& in, HCNode* previousNode);

    void uniquerebuild(istream& in);

    void rebuild(istream& in);

    void build(const vector<unsigned int>& freqs);

    void encode(byte symbol, BitOutputStream& out) const;

    void encode(byte symbol, ostream& out) const;

    byte decode(BitInputStream& in) const;

    byte decode(istream& in) const;
};

#endif  // HCTREE_HPP
