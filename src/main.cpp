#include <bits/stdc++.h>
#include "tree.h"
#include "cipher.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>

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

struct Task{
    string cmnd;
    string srcPath;
    string dstPath;
};

queue<struct Task> tasks;
pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void dirDfs(string srcPath, string dstPath, string cmnd){
    DIR *dir = opendir(srcPath.c_str());

    // Base case -> regular file
    if(dir == NULL){
        pthread_mutex_lock(&locker);
        tasks.push({cmnd, srcPath, dstPath});
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&locker);
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

bool producing = true;

void* routine(void* arg){
    while(true){
        pthread_mutex_lock(&locker);

        while(tasks.empty() && producing) pthread_cond_wait(&cond, &locker); 
        if(tasks.empty() && !producing) {
            pthread_mutex_unlock(&locker);
            break;
        }

        Task top = tasks.front();
        tasks.pop();

        pthread_mutex_unlock(&locker);

        if(top.cmnd == "zip"){
            zip(top.srcPath, top.dstPath);
        }else{
            unzip(top.srcPath, top.dstPath);
        }

    }

    return nullptr;
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

    pthread_t ths[8];
    for(int i=0;i<8;i++) pthread_create(ths+i, NULL, &routine, NULL);

    dirDfs(inputPath, outputPath, cmnd);
    pthread_mutex_lock(&locker);
    producing = false;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&locker);

    for(int i=0;i<8;i++) pthread_join(ths[i], nullptr);

    return 0;
}