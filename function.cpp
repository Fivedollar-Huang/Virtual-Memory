#include "function.hpp"

int PM[524288];
int D[1024][512];
int F[1022];
std::vector<int> PMchanges;
std::vector<int> Dchanges;
void initPM(std::string fileName){

    HandleFile file(fileName);
    int *arr;
    try{
        // std::cout << "TRY" << std::endl;
        while((arr = file.getLineOne())!= NULL){
            // std::cout << arr[0] << ", " << arr[1] << ", " << arr[2] << std::endl;
            PM[2*arr[0]] = arr[1];
            PM[2*arr[0]+1] = arr[2];

            PMchanges.push_back(2*arr[0]);
            PMchanges.push_back(2*arr[0]+1);

            F[arr[2]-2] = 1;
        }
        while((arr = file.getLineTwo()) != NULL){
            // std::cout <<"ARRAY\t:\t" <<  arr[0] << ", " << arr[1] << ", " << arr[2] << std::endl;
            int frameNum = PM[2*arr[0]+1];
            F[arr[2]-2] = 1;
            if( frameNum < 0){
                // store to disk
                // std::cout << -PMidx << ", " << arr[1] << std::endl;
                D[-frameNum][arr[1]] = arr[2];
                if(std::find(Dchanges.begin(), Dchanges.end(), -frameNum) == Dchanges.end()){
                    Dchanges.push_back(-frameNum);
                }
                continue;
            }
            PM[ frameNum*512 +arr[1] ] = arr[2];
            PMchanges.push_back(frameNum*512 +arr[1]);
        }
    }
    catch(std::exception e){
        std::cout <<"______________AN ERROR OCCURED___________________"<<std::endl;
        std::cout << e.what()<<std::endl;
    }
    // std::cout << "\nNext free frame is: " << takeFreeFrame() << std::endl<<std::endl;
}

void readVM(std::string fileName)
{
    HandleFile file(fileName);
    int num;
    int spw[4];
    try{
        while((num = file.getNextNum()) != -1){
            deriveVM(num, spw);
            // std::cout << "Num: " << num << std::endl;
            // std::cout << "Derived SPW:\t" << spw[0] << ", " << spw[1] << ", " << spw[2] << ", " << spw[3]<<std::endl;

            if(spw[3] >= PM[2*spw[0]]){
                file.put(-1);
                // std::cout << spw[3] << ", " << PM[2*spw[0]] << ", " << spw[0] << std::endl;
                std::cout << "___ERROR___  \n\t** pw < PM[2s] **" << std::endl;
            }
            else{
                if(PM[2*spw[0] +1] < 0){
                    // page fault: PT is not resident
                    // std::cout << std::endl;
                    std::cout << "___PT fault___"<<std::endl;
                    // std::cout << std::endl;
                    int freeFrame = takeFreeFrame();
                    read_block(PM[2*spw[0]+1], freeFrame);
                    PM[2*spw[0]+1] = freeFrame;
                }
                if(PM[PM[2*spw[0] +1]*512 + spw[1]] < 0){
                    // page fault: page is not resident
                    // std::cout << std::endl;
                    std::cout << "___Page fault___"<<std::endl;
                    // std::cout << std::endl;
                    int freeFrame = takeFreeFrame();
                    read_block(PM[PM[2*spw[0] +1]*512 + spw[1]], freeFrame);
                    PM[PM[2*spw[0] +1]*512 + spw[1]] = freeFrame;

                }
                // printStuff();
                file.put(PM[ PM[2*spw[0] + 1]*512 + spw[1]]*512 + spw[2]);
                // std::cout << "\tResult: " << PM[ PM[2*spw[0] + 1]*512 + spw[1]]*512 + spw[2] <<std::endl;
            }

        }
    }
    catch(std::exception e){
        std::cout <<"___________________AN ERROR OCCURED___________________"<<std::endl;
        std::cout << e.what()<<std::endl;
    }
}

void deriveVM(int num, int (&result)[4])
{
    result[3] = num& 0X3FFFF;
    result[2] = num& 0X1FF;
    num >>= 9;
    result[1] = num& 0X1FF;
    result[0] = num>>9;
}

void read_block(int b, int m)
{
    std::cout << "Reading block: " << -b << " into frame: " << m << std::endl;
    for(int i = 0; i < 512; i++){
        if(D[-b][i] != 0){
            PM[m*512+i] = D[-b][i];
            PMchanges.push_back(m*512+i);
            // std::cout << m*512 + i << ", " << D[-b][i]<<std::endl;
        }
    }
}

void write_block(int b, int m)
{
}

int takeFreeFrame()
{
    for(int i = 0; i < 1022; i++){
        if(F[i] == 0){
            F[i] = 1;
            return i+2;
        }
    }
    return 0;
}

void printStuff()
{
    std::cout << "========" << std::endl;
    std::cout << "PM: " << std::endl;
    std::sort(PMchanges.begin(), PMchanges.end());
    for (int i = 0; i < PMchanges.size(); i++)
    {
        std::cout << PMchanges[i] << "\t:\t"<< PM[PMchanges[i]] << std::endl;
    }
    std::cout << "========" << std::endl;
    std::cout << "Disk: " << std::endl;
    std::sort(Dchanges.begin(), Dchanges.end());
    for (int i = 0; i < Dchanges.size(); i++)
    {
        std::cout << Dchanges[i] << "\t:\t";
        for(int j = 0; j < 512; j++){
            if(D[Dchanges[i]][j] != 0)
                std::cout << "(" << j << ", " << D[Dchanges[i]][j] << "), ";
        }
        std::cout << std::endl;
    }
}
