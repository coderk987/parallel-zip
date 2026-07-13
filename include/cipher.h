#pragma once

#include "tree.h"
#include <string>

void encodeFile(std::string s, TreeNode* root);
std::string decodeFile(std::string s, TreeNode* root, int len);
void serializeTree(TreeNode *root, std::string &serial);
TreeNode* deserializeTree(std::string serial, int &idx);