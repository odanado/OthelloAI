#ifndef INCLUDE_OTHELLOAI_ENDGAME_AI_HPP
#define INCLUDE_OTHELLOAI_ENDGAME_AI_HPP

#include "OthelloBoard.hpp"
#include <cassert>
#include <vector>

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
        std::vector<std::pair<i64,CellType>> orderedValidCells;
        for(const auto &vaildCell : vaildCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;
            putStone(&boardTmp1, &boardTmp2, vaildCell.first, vaildCell.second);
            i64 evalValue = -dfs(boardTmp2, boardTmp1, 4, -beta, -alpha);
            orderedValidCells.emplace_back(evalValue, vaildCell);
        }

        sort(orderedValidCells.begin(), orderedValidCells.end(), 
                [](const std::pair<i64, CellType> &cell1, const std::pair<i64, CellType> &cell2) {
                    return cell1.first > cell2.first;
                });


        for(const auto &orderedValidCell : orderedValidCells) {
            auto vaildCell = orderedValidCell.second;
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
        i64 s;
        i64 a=alpha;
        i64 b=beta;
        bool first=true;

        for(const auto &vaildCell : vaildCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;
            putStone(&boardTmp1, &boardTmp2, vaildCell.first, vaildCell.second);
            s = -dfs(boardTmp2, boardTmp1, depth-1, -b, -a);
            if(s>a && s<beta && depth>1 && !first) a = -dfs(boardTmp2, boardTmp1, depth-1, -beta, -s);

            a=std::max(a,s);
            if(a>=beta) return a;
            b=a+1;
            first=false;
        }

        return a;
    }
};

} // end OthelloAI

#endif

