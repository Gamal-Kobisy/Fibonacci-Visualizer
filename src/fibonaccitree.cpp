#include "fibonaccitree.h"

FibonacciTree::FibonacciTree() 
    : root(nullptr), useMemo(false), totalCalls(0), memoizedCalls(0) {
}

FibonacciTree::~FibonacciTree() {
    reset();
}

void FibonacciTree::buildTree(int n, bool useMemoization) {
    reset();
    useMemo = useMemoization;
    totalCalls = 0;
    memoizedCalls = 0;
    
    if (useMemoization) {
        root = buildTreeWithMemo(n, 0, nullptr);
    } else {
        root = buildTreeRecursive(n, 0, nullptr);
    }
}

void FibonacciTree::reset() {
    if (root) {
        delete root;
        root = nullptr;
    }
    memoTable.clear();
    totalCalls = 0;
    memoizedCalls = 0;
}

TreeNode* FibonacciTree::buildTreeRecursive(int n, int depth, TreeNode* parent) {
    totalCalls++;
    
    TreeNode* node = new TreeNode(n, depth, parent);
    
    if (n <= 1) {
        node->result = n;
        node->isComputed = true;
        return node;
    }
    
    node->left = buildTreeRecursive(n - 1, depth + 1, node);
    node->right = buildTreeRecursive(n - 2, depth + 1, node);
    
    node->result = node->left->result + node->right->result;
    node->isComputed = true;
    
    return node;
}

TreeNode* FibonacciTree::buildTreeWithMemo(int n, int depth, TreeNode* parent) {
    totalCalls++;
    
    TreeNode* node = new TreeNode(n, depth, parent);
    
    // Check if already memoized
    if (memoTable.find(n) != memoTable.end()) {
        node->result = memoTable[n];
        node->isComputed = true;
        node->isMemoized = true;
        memoizedCalls++;
        return node;
    }
    
    if (n <= 1) {
        node->result = n;
        node->isComputed = true;
        memoTable[n] = n;
        return node;
    }
    
    node->left = buildTreeWithMemo(n - 1, depth + 1, node);
    node->right = buildTreeWithMemo(n - 2, depth + 1, node);
    
    node->result = node->left->result + node->right->result;
    node->isComputed = true;
    memoTable[n] = node->result;
    
    return node;
}

std::vector<TreeNode*> FibonacciTree::getAllNodes() const {
    std::vector<TreeNode*> nodes;
    if (root) {
        collectNodes(root, nodes);
    }
    return nodes;
}

void FibonacciTree::collectNodes(TreeNode* node, std::vector<TreeNode*>& nodes) const {
    if (!node) return;
    
    nodes.push_back(node);
    collectNodes(node->left, nodes);
    collectNodes(node->right, nodes);
}
