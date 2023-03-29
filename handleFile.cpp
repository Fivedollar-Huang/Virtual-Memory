#include "handleFile.hpp"

HandleFile::HandleFile(std::string fileName)
{
    idx[0] = idx[1] = 0;
    std::ifstream f(fileName);
    if(!getline(f, lines[0])){
        // std::cout << "****** Can not read line One ******";
        return;
    }
    clearLeadingSpaces(0);
    if(!getline(f, lines[1]) || lines[1].size() == 0){
        // std::cout << "****** Can not read line Two ******";
        out.open("output.txt");
        // std::cout << "---Opened output.txt file---"<<std::endl;
        return;
    }
    clearLeadingSpaces(1);
}

int *HandleFile::getLineOne()
{
    return getLine(0);
}

int *HandleFile::getLineTwo()
{
    return getLine(1);
}

int *HandleFile::getLine(int num)
{
    std::string s = lines[num];

    // Assume all input are triplets;
    int val = getNextNum(num);
    if(val == -1) return NULL;
    tempResult[0] = val;
    for(int i = 1; i < 3; i++){
        val = getNextNum(num);
        tempResult[i] = val;
    }
    return tempResult;
}

int HandleFile::getNextNum(int num)
{
    std::string val = "";
    std::string s = lines[num];
    if(idx[num] >= s.size()){
        return -1;
    }
    while(s[idx[num]] != ' ' && idx[num] < s.size()){
        val += s[idx[num]];
        idx[num]++;
    }
    clearLeadingSpaces(num);
    return std::stoi(val);
}

void HandleFile::clearLeadingSpaces(int num)
{
    std::string s = lines[num];
    while(idx[num] < s.size()){
        if(s[idx[num]] != ' '){
            break;
        }
        idx[num]++;
    }
}

void HandleFile::put(int num)
{
    out << num << ' ';
}


void HandleFile::closeFile()
{
    if(out.is_open())
        out.close();
}
