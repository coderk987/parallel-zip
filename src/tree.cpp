#include "tree.h"
#include <bits/stdc++.h>

int splitidx(std::vector<std::pair<int,char>> &txtdata, int l, int r){
    int sum = 0;
    for(int i=l;i<=r;i++) sum+=txtdata[i].first;
    int cur=0;
    sum/=2;
    for(int i=r;i>=l;i--){
        cur+=txtdata[i].first;
        if(sum-cur <= 0){
            return i;
        }
    }
    
    return (l+r)/2;
}

TreeNode* makeTree(int l,int r, std::vector<std::pair<int,char>> &txtdata){
    if (l > r) return nullptr;
    if(l==r){
        TreeNode *node = new TreeNode;
        node->val = txtdata[l].second;
        node->right = nullptr;
        node->left = nullptr;
        return node;
    }

    int mid = splitidx(txtdata, l, r);
    TreeNode *root = new TreeNode;
    root->val = '*';
    root->left = makeTree(l, mid-1, txtdata);
    root->right = makeTree(mid, r, txtdata);
    return root;
}

void debug_dfs(TreeNode *root){
    if(root==nullptr) return;

    std::cout<<root->val<<" ";
    debug_dfs(root->right);
    debug_dfs(root->left);
}

void serializeTree(TreeNode *root, std::string &serial){
    if(root==nullptr){
        serial+="1N";
        return;
    }
    serial+='0';
    serial+=root->val;
    serializeTree(root->left, serial);
    serializeTree(root->right, serial);
}

TreeNode* deserializeTree(std::string serial, int &idx){
    //std::cout<<"I am at "<<idx<<std::endl;
    if(serial[idx-1]=='1'){
        idx+=2;
        return nullptr;
    }
    TreeNode* node = new TreeNode;
    node->val = serial[idx];
    idx+=2;
    node->left = deserializeTree(serial, idx);
    node->right = deserializeTree(serial, idx);
    return node;
}