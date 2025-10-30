#ifndef FIBONACCITREE_H
#define FIBONACCITREE_H

#include <memory>
#include <unordered_map>
#include <vector>

// TreeNode represents a single node in the Fibonacci recursion tree.
// Ownership: Each node owns its children (left and right) and is responsible
// for their deletion. The parent pointer is non-owning and used for traversal only.
struct TreeNode {
    int value;
    int result;
    bool isComputed;
    bool isMemoized;
    int depth;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;  // Non-owning pointer to parent node
    
    TreeNode(int val, int d, TreeNode* p = nullptr)
        : value(val), result(-1), isComputed(false), isMemoized(false), 
          depth(d), left(nullptr), right(nullptr), parent(p) {}
    
    ~TreeNode() {
        delete left;
        delete right;
    }
};

class FibonacciTree {
public:
    FibonacciTree();
    ~FibonacciTree();
    
    void buildTree(int n, bool useMemoization = false);
    void reset();
    
    TreeNode* getRoot() const { return root; }
    const std::unordered_map<int, int>& getMemoTable() const { return memoTable; }
    
    int getTotalCalls() const { return totalCalls; }
    int getMemoizedCalls() const { return memoizedCalls; }
    
    std::vector<TreeNode*> getAllNodes() const;
    
private:
    TreeNode* buildTreeRecursive(int n, int depth, TreeNode* parent);
    TreeNode* buildTreeWithMemo(int n, int depth, TreeNode* parent);
    void collectNodes(TreeNode* node, std::vector<TreeNode*>& nodes) const;
    
    TreeNode* root;
    std::unordered_map<int, int> memoTable;
    bool useMemo;
    int totalCalls;
    int memoizedCalls;
};

#endif // FIBONACCITREE_H
