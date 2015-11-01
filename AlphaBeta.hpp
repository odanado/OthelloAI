#ifndef INCLUDE_ALPHA_BETA_HPP
#define INCLUDE_ALPHA_BETA_HPP

#include "Config.hpp"
#include "OthelloBoard.hpp"
#include <random>
#include <map>
#include <algorithm>
#include <cstdio>

namespace OthelloAI {

template<class EvalFunc>
class AlphaBeta {
public:
    AlphaBeta(CellState myColor, CellState enemyColor, i32 depth) : 
        myColor(myColor), enemyColor(enemyColor), DEPTH_LEVAL(depth) {
            assert(myColor != CellState::NONE);
            assert(enemyColor != CellState::NONE);
            assert(myColor != enemyColor);
            mt.seed(0);
        }

    template<CellState Color1, CellState Color2>
    CellType operator()(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) noexcept {

        static_assert(Color1 != Color2,"");
        assert(Color1 == myColor);
        assert(Color2 == enemyColor);

        CellType result(-1,-1);

        auto vaildCells = makeVaildCells(board1, board2);

        if(!vaildCells.size()) {
            return result;
        }
        std::shuffle(vaildCells.begin(), vaildCells.end(), mt);
        i64 alpha = minValue<i64>();
        i64 beta = maxValue<i64>();

        for(const auto &vaildCell : vaildCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;

            i64 evalValue  = minValue<i64>();
            putStone(&boardTmp1, &boardTmp2, vaildCell.first, vaildCell.second);
            evalValue = -dfs(boardTmp2, boardTmp1, DEPTH_LEVAL, -beta, -alpha);

            if(alpha < evalValue) {
                alpha = evalValue;
                result = vaildCell;
            }
        }

        return result;
    }

private:
    CellState myColor, enemyColor;
    EvalFunc evalFunc;
    const i32 DEPTH_LEVAL;
    std::mt19937 mt;

    template<CellState Color1, CellState Color2>
    i64 dfs(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2, i32 depth, i64 alpha, i64 beta) noexcept {

        static_assert(Color1 != Color2,"");

        if(myColor == Color1 && (isFinished(board1, board2) || depth < 0)) {
            return evalFunc(board1, board2);
        }

        auto vaildCells = makeVaildCells(board1, board2);
        if(!vaildCells.size()) {
            return -dfs(board2, board1, depth-1, -beta, -alpha);
        }

        for(const auto &vaildCell : vaildCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;
            putStone(&boardTmp1, &boardTmp2, vaildCell.first, vaildCell.second);
            alpha = std::max(alpha, -dfs(boardTmp2, boardTmp1, depth-1, -beta, -alpha));
            if(alpha >= beta) {
                return alpha;
            }
        }

        return alpha;
    }
};
} // end OthelloAI

#endif

