#pragma once
#include <iostream>
#include <map>
#include <vector>

#include "handleFile.hpp"

void initPM(std::string fileName);

void readVM(std::string fileName);

void deriveVM(int num, int (&result)[4]);


void read_block(int b, int m);
void write_block(int b, int m);

int takeFreeFrame();

void printStuff();
