#include <bits/stdc++.h>
#include "tree.h"
#include "cipher.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

void getData(vector<pair<int, char>> &txtdata, string filepath){
    vector<int> freq(256, 0);
    char c;
    int fd = open(filepath.c_str(), O_RDONLY);

    while(read(fd, &c, 1) > 0){
        freq[static_cast<unsigned char>(c)]++;
    }

    close(fd);

    for(int i = 0; i < 256; i++){
        if(freq[i] > 0){
            txtdata.push_back({freq[i], static_cast<char>(i)});
        }
    }

    sort(txtdata.begin(), txtdata.end());
}

void zip(string srcPath, string dstPath){
    vector<pair<int, char>> txtdata;
    getData(txtdata, srcPath);

    TreeNode *root = makeTree(0, txtdata.size() - 1, txtdata);
    encodeFile(srcPath, dstPath, root);
}

void unzip(string srcPath, string dstPath){
    decodeFile(srcPath, dstPath);
}

void dirDfs(string srcPath, string dstPath, string cmnd){
    DIR *dir = opendir(srcPath.c_str());

    // Base case -> regular file
    if(dir == NULL){
        if(cmnd == "zip"){
            zip(srcPath, dstPath);
        }else{
            unzip(srcPath, dstPath);
        }
        return;
    }

    // Create destination directory
    mkdir(dstPath.c_str(), 0755);

    struct dirent *entry;

    while((entry = readdir(dir)) != NULL){
        string entryName = entry->d_name;

        if(entryName == "." || entryName == "..")
            continue;

        string newSrc = srcPath + "/" + entryName;
        string newDst = dstPath + "/" + entryName;

        dirDfs(newSrc, newDst, cmnd);
    }

    closedir(dir);
}

int main(int argc, char* argv[]){
    if(argc < 3)
        return 0;

    string cmnd = argv[1];
    string inputPath = argv[2];

    string outputPath;

    if(cmnd == "zip"){
        outputPath = inputPath + "_Compressed";
    }else if(cmnd == "unzip"){
        outputPath = inputPath + "_Uncompressed";
    }else{
        cout << "Unknown command\n";
        return 0;
    }

    mkdir(outputPath.c_str(), 0755);

    dirDfs(inputPath, outputPath, cmnd);

    return 0;
}