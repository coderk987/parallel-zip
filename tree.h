#pragma once

#include <vector>
#include <utility>

struct TreeNode {
    char val;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* makeTree(int l, int r, std::vector<std::pair<int,char>> &txtdata);
void debug_dfs(TreeNode* root);