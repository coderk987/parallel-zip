#include "tree.h"
#include "cipher.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include "bitio.h"
#include <sys/xattr.h>

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

    int fdr = open(s.c_str(), O_RDONLY);
    char c;
    int fdw = open("comp.txt", O_WRONLY | O_CREAT, 0644);
    //cout << fdr << " " << fdw << endl;
    BitWriter bw(fdw);
    int writes = 0;
    while(read(fdr, &c, 1) > 0) {
        for(char &c:encodeCipher[c]){
            writes++;
            bw.writeBit((c=='1'));
        }
    }

    //cout<<"wrote bits\n";
    int padding = (8 - writes % 8) % 8;
    for(int i=0;i<padding;i++){
        bw.writeBit(0);
    }

    string treestr;
    serializeTree(root, treestr);
    setxattr("comp.txt", "user.comp.tree", treestr.data(), treestr.size(), 0);
    setxattr("comp.txt", "user.comp.writes", &writes, sizeof(writes), 0);
    //cout<<"encoded\n";
}

void decodeFile(string filepath){
    ssize_t treeSize = getxattr(filepath.data(), "user.comp.tree", NULL, 0);
    string treestr(treeSize, '\0'); 
    getxattr(filepath.data(), "user.comp.tree", treestr.data(), treestr.size());
    int writes;
    getxattr(filepath.data(), "user.comp.writes", &writes, sizeof(writes));
    //cout<<"got attr\n";
    int idxtmp = 0;
    TreeNode* root = deserializeTree(treestr, idxtmp);
    createCipher(root, "", false);
    //cout<<"cipher+tree done\n";
    int fdr = open("comp.txt", O_RDONLY);
    string cur = "";
    int curbit;
    BitReader br(fdr);
    int curreads = 0;
    int fdw = open("./og.txt", O_WRONLY | O_CREAT, 0644);
    //cout << fdr << " " << fdw << endl;
    //cout<<"\nwrites: "<<writes<<endl;
    while((curbit=br.readBit()) >= 0){
        cur+=(curbit==1 ? '1' :'0');
        //cout << curreads << " " << curbit << " " << cur << endl;
        if(decodeCipher.contains(cur)){
            char decoded_cur = decodeCipher[cur];
            //cout<<"decoding "<<decoded_cur<<endl;
            write(fdw, &decoded_cur, 1);
            cur="";
        }
        if(curreads==writes) break;
        curreads++;
    }
    //cout<<"decoded\n";
}