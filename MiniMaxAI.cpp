#include "MiniMaxAI.hpp"
#include <cassert>
#include <limits>
#include <iostream>

namespace OthelloAI {

const u32 MiniMaxAI::DEPTH_LEVAL = 6;

MiniMaxAI::MiniMaxAI(CellState myColor, CellState enemyColor) : 
    myColor(myColor), enemyColor(enemyColor) {
        assert(myColor != CellState::NONE);
        assert(enemyColor != CellState::NONE);
        assert(myColor != enemyColor);
    }

template<CellState Color1, CellState Color2>
CellType MiniMaxAI::operator()(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) noexcept {
    static_assert(Color1 != Color2,"");
    assert(Color1 == myColor);
    assert(Color2 == enemyColor);
    CellType result(-1,-1);

    auto vaildCells = makeVaildCells(board1, board2);

    if(!vaildCells.size()) {
        return result;
    }
    i64 maxi = std::numeric_limits<i64>::min();

    for(const auto &vaildCell : vaildCells) {
        OthelloBoard<Color1> boardTmp1 = board1;
        OthelloBoard<Color2> boardTmp2 = board2;

        putStone(&boardTmp1, &boardTmp2, vaildCell.first, vaildCell.second);
        i64 evalValue = dfs(boardTmp2, boardTmp1, 0);

        if(maxi < evalValue) {
            maxi = evalValue;
            result = vaildCell;
        }
    }

    return result;
}


template<CellState Color1, CellState Color2>
i64 MiniMaxAI::dfs(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2, u32 depth) noexcept {
    static_assert(Color1 != Color2,"");

    if(depth > DEPTH_LEVAL && Color1 == myColor) {
        return eval(board1, board2);
    }
    i64 result;

    if(Color1 == myColor) {
        result = std::numeric_limits<i64>::min();
    }
    else {
        result = std::numeric_limits<i64>::max();
    }

    auto vaildCells = makeVaildCells(board1, board2);
    if(!vaildCells.size()) {
        return dfs(board2, board1, depth + 1);
    }
    for(const auto &vaildCell : vaildCells) {
        OthelloBoard<Color1> boardTmp1 = board1;
        OthelloBoard<Color2> boardTmp2 = board2;
        putStone(&boardTmp1, &boardTmp2, vaildCell.first, vaildCell.second);
        i32 evalValue = dfs(boardTmp2, boardTmp1, depth + 1);
        if(Color1 == myColor && result < evalValue) {
            result = evalValue;
        }
        if(Color1 == enemyColor && result > evalValue) {
            result = evalValue;
        }
    }

    return result;
}

template<CellState Color1, CellState Color2>
i64 MiniMaxAI::eval(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) noexcept {
    i64 result = board1.countStone();
    result -= (i64) board2.countStone();

    if(board1.existStone(0,0)) result += 2;
    if(board1.existStone(0,7)) result += 2;
    if(board1.existStone(7,0)) result += 2;
    if(board1.existStone(7,7)) result += 2;
    if(board2.existStone(0,0)) result -= 2;
    if(board2.existStone(0,7)) result -= 2;
    if(board2.existStone(7,0)) result -= 2;
    if(board2.existStone(7,7)) result -= 2;

    return result;
}

template CellType MiniMaxAI::operator()<CellState::BLACK, CellState::WHITE>(const OthelloBoard<CellState::BLACK>&, const OthelloBoard<CellState::WHITE>&) noexcept;
template CellType MiniMaxAI::operator()(const OthelloBoard<CellState::WHITE>&, const OthelloBoard<CellState::BLACK>&) noexcept;

template i64 MiniMaxAI::dfs(const OthelloBoard<CellState::BLACK>&, const OthelloBoard<CellState::WHITE>&,u32) noexcept;
template i64 MiniMaxAI::dfs(const OthelloBoard<CellState::WHITE>&, const OthelloBoard<CellState::BLACK>&,u32) noexcept;

template i64 MiniMaxAI::eval(const OthelloBoard<CellState::BLACK>&, const OthelloBoard<CellState::WHITE>&) noexcept;
template i64 MiniMaxAI::eval(const OthelloBoard<CellState::WHITE>&, const OthelloBoard<CellState::BLACK>&) noexcept;


} // end OhelloAI
