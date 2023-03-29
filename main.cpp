#include <iostream>
#include "handleFile.hpp"
#include "function.hpp"


int main(int argc, char const *argv[])
{
    /* code */
    std::string initFile = "init.txt";
    std::string inFile = "va.txt";
    if(argc == 3){
        initFile = argv[1];
        inFile = argv[2];
    }
    initPM(initFile);
    // printStuff();
    readVM(inFile);
    // printStuff();
    return 0;
}
