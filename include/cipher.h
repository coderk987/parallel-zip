#pragma once

#include "tree.h"
#include <string>

std::string encodeFile(std::string s, TreeNode* root);
std::string decodeFile(std::string s, TreeNode* root, int len);