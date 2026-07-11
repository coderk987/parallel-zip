#include <bits/stdc++.h>
#include "tree.h"
#include "cipher.h"

using namespace std;

int main(){
    string s = "hellowowowo";

    vector<int> freq(256,0);
    for(char &c: s){
        freq[static_cast<int>(c)]++;
    }
    vector<pair<int, char>> txtdata;
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

    TreeNode *root = makeTree(0, txtdata.size()-1, txtdata);
    debug_dfs(root);
    cout<<endl;

    string zip = encodeFile(s, root);
    cout<<zip<<endl;
    string unzip = decodeFile(zip, root, s.length());
    cout<<unzip<<endl;

    return 0;
}