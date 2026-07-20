#pragma once

#include "tree.h"
#include <string>

void encodeFile(std::string srcpath, std::string dstpath, TreeNode* root);
void decodeFile(std::string srcpath, std::string dstpath);