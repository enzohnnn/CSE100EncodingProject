#include "HCTree.hpp"
#include <iostream>
#include <stack>
using namespace std;

void HCTree::preorderTraversal(HCNode* current, string& str) {
    if (current == nullptr) {
        return;
    }
    // preorder works Visit,Left,Right
    if (current->c0 == nullptr && current->c1 == nullptr) {
        // if leaf, append 1 + symbol
        str.push_back('1');
        str.push_back((char)(current->symbol));
    } else {
        str.push_back('0');
    }
    preorderTraversal(current->c0, str);
    preorderTraversal(current->c1, str);
}

string HCTree::buildString() {
    string str;
    preorderTraversal(root, str);
    return str;
}

HCNode* HCTree::rebuildRecursively(istream& in, HCNode* previousNode) {
    char c;
    char c2;
    // if i read in a 1, means it is a leaf
    in.get(c);
    if (c == '1') {
        in.get(c2);
        return new HCNode(0, c2, nullptr, nullptr, previousNode);
    } else {  // if it isnt a 1 it is a 0, idicating an internal node
        HCNode* newNode = new HCNode(0, 0, nullptr, nullptr, previousNode);
        newNode->c0 = rebuildRecursively(in, newNode);
        newNode->c1 = rebuildRecursively(in, newNode);
        return newNode;
    }
}

/**
 * Method to rebuild the tree with only ONE child
 */
void HCTree::uniquerebuild(istream& in) {
    char c;
    in.get(c);  // gets root node
    in.get(c);  // gets 1
    in.get(c);  // gets the symbol
    HCNode* newRoot = new HCNode(0, 0, nullptr, nullptr, nullptr);
    HCNode* child = new HCNode(0, c, nullptr, nullptr, newRoot);
    newRoot->c0 = child;
    root = newRoot;
}

/**
 * Method to rebuild the tree with a unique encoded tree
 */
void HCTree::rebuild(istream& in) {
    root = rebuildRecursively(in, nullptr);
    HCNode* test = root;
    while (1) {
        if (test->c1 == nullptr) {
            // cout << (int)test->symbol << endl;
            return;
        }
        test = test->c1;
    }
}

void myHelperRemove(HCNode* current) {
    if (current == nullptr) {
        return;
    }
    // delete subchildren
    myHelperRemove(current->c0);
    myHelperRemove(current->c1);
    delete current;
    current = nullptr;
}

/* TODO: Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() { myHelperRemove(root); }

/**
 * TODO: Build the HCTree from the given frequency vector. You can assume the
 * vector must have size 256 and each value at index i represents the frequency
 * of char with ASCII value i. Only non-zero frequency symbols should be used to
 * build the tree. The leaves vector must be updated so that it can be used in
 * encode() to improve performance.
 *
 * When building the HCTree, you should use the following tie-breaking rules to
 * match the output from reference solution in checkpoint:
 *
 *    1. HCNode with lower count should have higher priority. If count is the
 * same, then HCNode with a larger ascii value symbol should have higher
 * priority. (This should be already defined properly in the comparator in
 * HCNode.hpp)
 *    2. When popping two highest priority nodes from PQ, the higher priority
 * node will be the ‘c0’ child of the new parent HCNode.
 *    3. The symbol of any parent node should be taken from its 'c0' child.
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    // create my priority queue
    std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> mypq;
    // create my forest
    vector<HCNode*> forest;
    // push the frequencies onto my priority queue
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            HCNode* newNode =
                new HCNode(freqs[i], i, nullptr, nullptr, nullptr);
            mypq.push(newNode);
            // also add these leaves onto my leaves vector
            leaves.push_back(newNode);
        }
    }
    // special case for only one thing in pq
    if (mypq.size() == 1) {
        HCNode* emptyRoot = new HCNode(0, 0, mypq.top(), nullptr, nullptr);
        root = emptyRoot;
        mypq.top()->p = root;
        mypq.pop();
        // cout << "what is my symbol: " << (int)root->c0->symbol << endl;
        return;
    }
    while (mypq.size() > 1) {
        HCNode* combinedNode = new HCNode(0, 0, nullptr, nullptr, nullptr);
        int count1 = mypq.top()->count;
        byte symbol1 = mypq.top()->symbol;
        combinedNode->symbol =
            mypq.top()->symbol;  // symbol of parent is taken from c0 child
        combinedNode->c0 = mypq.top();
        mypq.top()->p = combinedNode;
        mypq.pop();
        int count2 = mypq.top()->count;
        combinedNode->count = count1 + count2;
        combinedNode->c1 = mypq.top();
        mypq.top()->p = combinedNode;
        mypq.pop();
        mypq.push(combinedNode);
    }
    // The final HCNode in my priority queue is my root
    root = mypq.top();
}

/**
 * TODO: Write the encoding bits of the given symbol to the ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    HCNode* leaf = nullptr;
    for (int i = 0; i < leaves.size(); i++) {
        if (leaves[i]->symbol == symbol) {
            leaf = leaves[i];
        }
    }
    HCNode* parent = leaf->p;
    std::stack<int> code;
    while (parent != nullptr) {
        if (leaf == parent->c0) {
            code.push(0);
            leaf = leaf->p;
            parent = parent->p;
        } else {
            code.push(1);
            leaf = leaf->p;
            parent = parent->p;
        }
    }
    // cout << "what" << endl;
    while (!code.empty()) {
        // cout << "out" << endl;
        out.writeBit(code.top());
        code.pop();
    }
    // out.flush();
}

/**
 * TODO: Write the encoding bits of the given symbol to ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been called
 * beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    HCNode* leaf = nullptr;
    // cout << "yello" << endl;
    for (int i = 0; i < leaves.size(); i++) {
        if (leaves[i]->symbol == symbol) {
            leaf = leaves[i];
            // cout << "got the leaf" << endl;
        }
    }
    HCNode* parent = leaf->p;
    std::stack<char> code;
    while (parent != nullptr) {
        // cout << "while loop" << endl;
        if (leaf == parent->c0) {
            code.push('0');
            leaf = leaf->p;
            parent = parent->p;
        } else {
            code.push('1');
            leaf = leaf->p;
            parent = parent->p;
        }
    }
    while (!code.empty()) {
        out.put((char)code.top());
        code.pop();
    }
}

/**
 * TODO: Decode the sequence of bits (represented as a char of either '0' or
 * '1') from the istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(BitInputStream& in) const {
    HCNode* currentNode = root;
    while (true) {
        // cout << "inside loop" << endl;
        if (currentNode->c0 == nullptr && currentNode->c1 == nullptr) {
            // cout << "decode ended" << endl;
            return currentNode->symbol;
        }
        int bit = in.readBit();
        if (bit == 0) {
            currentNode = currentNode->c0;
        } else {
            currentNode = currentNode->c1;
        }
    }
    // cout << "decode ended" << endl;
    return ' ';
}

/**
 * TODO: Decode the sequence of bits (represented as char of either '0' or '1')
 * from istream to return the coded symbol. For this function to work, build()
 * must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const {
    HCNode* currentNode = root;
    char* buffer = new char[1];
    int i = 0;
    while (true) {
        if (currentNode->c0 == nullptr && currentNode->c1 == nullptr) {
            delete buffer;
            return currentNode->symbol;
        }
        in.read(buffer, 1);
        if (buffer[0] == '0') {
            currentNode = currentNode->c0;
        } else {
            currentNode = currentNode->c1;
        }
    }
    delete buffer;
    return ' ';
}
