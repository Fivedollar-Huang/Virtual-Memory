#pragma once
#include <iostream>
#include <fstream>

// Probably should not be class and just functions.
class HandleFile{
    std::ofstream out;

    std::string lines[2];
    int idx[2];

    int tempResult[3];

    public:
    HandleFile(std::string fileName);

    int *getLineOne();

    int *getLineTwo();

    int *getLine(int num);

    int getNextNum(int num=0);

    void clearLeadingSpaces(int num);

    void put(int num);

    void closeFile();
};

