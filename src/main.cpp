#include <bits/stdc++.h>
#include "tree.h"
#include "cipher.h"
#include <unistd.h>
#include <fcntl.h>

using namespace std;

const string filePath = "sample/hi.txt";

void getData(vector<pair<int, char>> &txtdata){
    vector<int> freq(256,0);
    char c;
    int fd = open(filePath.c_str(), O_RDONLY);
    while(read(fd, &c, 1) > 0){
        freq[static_cast<int>(c)]++;
    }

    for(int i=0;i<256;i++){
        if(freq[i]>0){
            pair<int,int> p = {freq[i], static_cast<char>(i)};
            txtdata.push_back(p);
        }
    }
    
    sort(txtdata.begin(), txtdata.end());
}

int main(){
    vector<pair<int, char>> txtdata;
    getData(txtdata);
    TreeNode *root = makeTree(0, txtdata.size()-1, txtdata);
    debug_dfs(root);
    encodeFile(filePath, root);
    string serial;
    serializeTree(root, serial);
    cout<<endl<<serial<<endl;
    int idx=1;
    TreeNode *test = deserializeTree(serial, idx);
    //debug_dfs(test);
    return 0;
}