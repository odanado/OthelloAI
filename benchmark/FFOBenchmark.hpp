#include <iostream>
#include <string>

#include "../EndGameAI.hpp"


namespace OthelloAI {
class FFOBenchmark {
public:
    std::string operator()(const std::string &fileName);
private:
    void input(const std::string &fileName, BlackBoard *blackBoard, WhiteBoard *whiteBoard, std::string *turn, std::string *testName);
};
} // OthelloAI

