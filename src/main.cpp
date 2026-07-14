#include <bits/stdc++.h>
#include "tree.h"
#include "cipher.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/xattr.h>

using namespace std;

void getData(vector<pair<int, char>> &txtdata,string filepath){
    vector<int> freq(256,0);
    char c;
    int fd = open(filepath.c_str(), O_RDONLY);
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

void zip(string filepath){
    vector<pair<int, char>> txtdata;
    getData(txtdata, filepath);
    TreeNode *root = makeTree(0, txtdata.size()-1, txtdata);
    encodeFile(filepath, root);
}

void unzip(string filepath){
    decodeFile(filepath);
}

int main(int argc, char* argv[]){
    if(argc < 3) return 0;

    string cmnd = argv[1];
    if(cmnd == "zip"){
        zip(argv[2]);
    }else if(cmnd =="unzip"){
        unzip(argv[2]);
    }

    return 0;
}