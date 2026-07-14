#pragma once

#include <vector>
#include <utility>
#include <string>

struct TreeNode {
    char val;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* makeTree(int l, int r, std::vector<std::pair<int,char>> &txtdata);
void debug_dfs(TreeNode* root);
void serializeTree(TreeNode *root, std::string &serial);
TreeNode* deserializeTree(std::string serial, int &idx);