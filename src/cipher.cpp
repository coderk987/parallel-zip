#include "tree.h"
#include "cipher.h"
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include "bitio.h"
#include <sys/xattr.h>

using namespace std;


void createCipher(TreeNode* root,string path, bool isEncode, map<char, string> *encodeCipher, map<string, char> *decodeCipher){
    if(root == nullptr) return;
    if(root->right==nullptr && root->left==nullptr){
        if(isEncode) (*encodeCipher)[root->val]=path;
        else (*decodeCipher)[path]=root->val;
        return;
    }

    createCipher(root->left, path+"0", isEncode, encodeCipher, decodeCipher);
    createCipher(root->right, path+"1", isEncode, encodeCipher, decodeCipher);
}

void encodeFile(string srcpath, string dstpath, TreeNode* root){
    map<char, string> encodeCipher;
    createCipher(root, "", true, &encodeCipher, nullptr);

    int fdr = open(srcpath.c_str(), O_RDONLY);
    char c;
    int fdw = open(dstpath.c_str(), O_WRONLY | O_CREAT, 0644);
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
    setxattr(dstpath.c_str(), "user.comp.tree", treestr.data(), treestr.size(), 0);
    setxattr(dstpath.c_str(), "user.comp.writes", &writes, sizeof(writes), 0);
    //cout<<"encoded\n";

    close(fdr);
    close(fdw);
}

void decodeFile(string srcpath, string dstpath){
    ssize_t treeSize = getxattr(srcpath.c_str(), "user.comp.tree", NULL, 0);
    string treestr(treeSize, '\0'); 
    getxattr(srcpath.c_str(), "user.comp.tree", treestr.data(), treestr.size());
    int writes;
    getxattr(srcpath.c_str(), "user.comp.writes", &writes, sizeof(writes));
    //cout<<"got attr\n";
    int idxtmp = 0;
    TreeNode* root = deserializeTree(treestr, idxtmp);

    map<string, char> decodeCipher;
    createCipher(root, "", false,nullptr,&decodeCipher);
    //cout<<"cipher+tree done\n";

    int fdr = open(srcpath.c_str(), O_RDONLY);
    string cur = "";
    int curbit;
    BitReader br(fdr);
    int curreads = 0;
    int fdw = open(dstpath.c_str(), O_WRONLY | O_CREAT, 0644);
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

    close(fdr);
    close(fdw);
    //cout<<"decoded\n";
}