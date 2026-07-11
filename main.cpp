#include <bits/stdc++.h>

using namespace std;

struct TreeNode{
    char val;
    TreeNode *right;
    TreeNode *left;
};

vector<pair<int, char>> txtdata;

TreeNode* makeTree(int l,int r){
    if(l==r){
        TreeNode *node = new TreeNode;
        node->val = txtdata[l].second;
        node->right = nullptr;
        node->left = nullptr;
        return node;
    }

    int mid = (l+r)/2; //temporary incorrect split
    TreeNode *root = new TreeNode;
    root->val = '*'; //temporary
    root->left = makeTree(l, mid);
    root->right = makeTree(mid+1, r);
    return root;
}

void dfs(TreeNode *root){
    if(root==nullptr) return;

    cout<<root->val<<" ";
    dfs(root->right);
    dfs(root->left);
}

int main(){
    string s = "hellowowowo";

    vector<int> freq(256,0);
    for(char &c: s){
        freq[static_cast<int>(c)]++;
    }

    for(int i=0;i<256;i++){
        if(freq[i]>0){
            pair<int,int> p = {freq[i], static_cast<char>(i)};
            txtdata.push_back(p);
        }
    }
    
    sort(txtdata.begin(), txtdata.end());
    for(auto &x: txtdata){
        cout<<x.second<<"=>"<<x.first<<endl;
    }

    TreeNode *root = makeTree(0, txtdata.size()-1);

    dfs(root);

    //read file
    //make tree
    //encode using tree to a binary file
    //convert binary file to text
    //add tree as header to new text file
    

    return 0;
}