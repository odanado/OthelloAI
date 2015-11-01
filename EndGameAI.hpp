#ifndef INCLUDE_OTHELLOAI_ENDGAME_AI_HPP
#define INCLUDE_OTHELLOAI_ENDGAME_AI_HPP

#include "OthelloBoard.hpp"
#include <cassert>

namespace OthelloAI {

class EndGameAI {
public:
    EndGameAI(CellState myColor = CellState::BLACK, CellState enemyColor = CellState::WHITE) :
        myColor(myColor), enemyColor(enemyColor), DEPTH_LEVEL(12) {
            assert(myColor != CellState::NONE);
            assert(enemyColor != CellState::NONE);
            assert(myColor != enemyColor);
        }

    void setPlayer(CellState myColor, CellState enemyColor) noexcept {
        this->myColor = myColor;
        this->enemyColor = enemyColor;
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

        i64 alpha = minValue<i64>();
        i64 beta = maxValue<i64>();

        for(const auto &vaildCell : vaildCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;

            i64 evalValue  = minValue<i64>();
            putStone(&boardTmp1, &boardTmp2, vaildCell.first, vaildCell.second);
            evalValue = -dfs(boardTmp2, boardTmp1, DEPTH_LEVEL, -beta, -alpha);

            if(alpha < evalValue) {
                alpha = evalValue;
                result = vaildCell;
            }
        }

        return result;
    }

private:
    CellState myColor, enemyColor;
    const i32 DEPTH_LEVEL;

    template<CellState Color1, CellState Color2>
    i64 dfs(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2, i32 depth, i64 alpha, i64 beta) noexcept {

        static_assert(Color1 != Color2,"");

        if(isFinished(board1, board2) || depth == 0) {
            i64 result;
            if(Color1 == myColor) result = countStone(board1);
            else result = -countStone(board2);
            return result;
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

