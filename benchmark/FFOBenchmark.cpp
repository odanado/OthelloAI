#include "FFOBenchmark.hpp"
#include <fstream>
#include <chrono>

namespace OthelloAI {

std::string FFOBenchmark::operator()(const std::string &fileName) {
    std::string result, turn, testName;
    BlackBoard blackBoard;
    WhiteBoard whiteBoard;
    input(fileName, &blackBoard, &whiteBoard, &turn, &testName);
    EndGameAI ai;

    if(turn == "Black") {
        ai.setPlayer(CellState::BLACK, CellState::WHITE);
    }
    else {
        ai.setPlayer(CellState::WHITE, CellState::BLACK);
    }

    CellType cell;
    auto start = std::chrono::system_clock::now();
    if(turn == "Black") {
        cell = ai(blackBoard, whiteBoard);
    }
    else {
        cell = ai(whiteBoard, blackBoard);
    }
    auto end = std::chrono::system_clock::now();

    double diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    result += "FFO Benchmark\n";
    result += testName + "\n";
    result += "turn: " + turn + "\n";

    result += "next Stone: ";
    result += "(" + std::to_string(cell.first) + ", " + std::to_string(cell.second) + ")\n";

    result += "time: " + std::to_string(diff) + "[ms]\n";
    return result;
}

void FFOBenchmark::input(const std::string &fileName, BlackBoard *blackBoard, WhiteBoard *whiteBoard, std::string *turn, std::string *testName) {
    std::ifstream fin(fileName, std::ios::in);
    std::string line;
    u64 b=0,w=0;

    std::getline(fin,line);

    for(std::size_t i = 0; i < line.size(); ++i) {
        if(line[i]=='X') b |= 1LL << i;
        if(line[i]=='O') w |= 1LL << i;
    }
    *blackBoard = BlackBoard(b);
    *whiteBoard = WhiteBoard(w);

    fin>>line;
    *turn = line;

    std::getline(fin,line);

    std::getline(fin,line);
    *testName = line;

}


} // end OthelloAI
