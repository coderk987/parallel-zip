#include "tree.h"
#include "cipher.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

map<char, string> encodeCipher;
map<string, char> decodeCipher;

void createCipher(TreeNode* root,string path, bool isEncode){
    if(root == nullptr) return;
    if(root->right==nullptr && root->left==nullptr){
        if(isEncode) encodeCipher[root->val]=path;
        else decodeCipher[path]=root->val;
        return;
    }

    createCipher(root->left, path+"0", isEncode);
    createCipher(root->right, path+"1", isEncode);
}

void encodeFile(string s, TreeNode* root){
    createCipher(root, "", true);

    string bincode="";
    int fd = open(s.c_str(), O_RDONLY);
    char c;
    while(read(fd, &c, 1) > 0) bincode+=encodeCipher[c];

    int padding = (8 - bincode.length() % 8) % 8;
    for(int i=0;i<padding;i++) bincode+='0';

    int fdw = open("comp.txt", O_WRONLY | O_CREAT, 0644);
    for(int i=0;i<bincode.length();i+=8){
        string binint = "";
        for(int j=0;j<8;j++){
            binint+=bincode[i+j];
        }
        char c = static_cast<char>(std::stoi(binint, nullptr, 2));
        write(fdw, &c, 1);
    }
}

string decodeFile(string s, TreeNode* root, int len){
    createCipher(root, "", false);
    string bincode = "";
    for(auto c:s){
        string binint = bitset<8>(c).to_string();
        bincode+=binint;
    }

    string txt = "";
    string cur = "";
    for(auto c:bincode){
        if(txt.length()==len) break;
        cur+=c;
        if(decodeCipher.contains(cur)){
            txt+=decodeCipher[cur];
            cur="";
        }
    }

    return txt;
}