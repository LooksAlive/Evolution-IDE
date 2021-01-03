#ifndef INDEXER_H
#define INDEXER_H

#include "clangd/index/Index.h"

class Indexer
{
public:
    Indexer();
    ~Indexer();

    void addFile();
    void removeFile();

    auto getAST();
    void loadIndexFile(const std::string &file_path);
    void saveIndexFile(const std::string &file_path);


    void updateChangedFile(const std::string &file_path); // update only 1 file
    void updateWholeIndexFile();                          // update all components

    auto getIndexData(); // no idea how it works yet so ...

};

#endif // INDEXER_H
