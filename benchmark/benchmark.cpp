#include <iostream>
#include "FFOBenchmark.hpp"

int main(int argc,char *argv[]) {
    if(argc != 2) {
        std::cout<<"pos ファイルを指定してください"<<std::endl;
        return 0;
    }
    std::string fileName(argv[1]);
    OthelloAI::FFOBenchmark benchmark;
    auto result = benchmark(fileName);
    std::cout<<result<<std::endl;


    return 0;
}
