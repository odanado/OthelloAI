#ifndef INCLUDE_OTHELLOAI_ENDGAME_AI_HPP
#define INCLUDE_OTHELLOAI_ENDGAME_AI_HPP

#include "OthelloBoard.hpp"
#include "EndGameEval.hpp"
#include <cassert>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <random>
#include <iostream>

namespace OthelloAI {

class EndGameAI {
public:
    EndGameAI(CellState myColor = CellState::BLACK, CellState enemyColor = CellState::WHITE, i32 depth = 20) :
        myColor(myColor), enemyColor(enemyColor), DEPTH_LEVEL(depth) {
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

        auto validCells = makeOrderedValidCells(board1, board2);

        if(!validCells.size()) {
            return result;
        }

        i64 alpha = minValue<i64>();
        i64 beta = maxValue<i64>();

        i64 a=alpha;
        i64 evalValue  = minValue<i64>();
        bool first=true;
        for(const auto &validCell : validCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;

            putStone(&boardTmp1, &boardTmp2, validCell.first, validCell.second);
            if(first) {
                evalValue=-dfs(boardTmp2, boardTmp1, DEPTH_LEVEL, -beta,-a);
            }
            else {
                evalValue=-dfs(boardTmp2, boardTmp1, DEPTH_LEVEL, -a-1,-a);
                if(evalValue>a && evalValue<beta) evalValue=-dfs(boardTmp2, boardTmp1, DEPTH_LEVEL, -beta,-evalValue);
            }

            a=std::max(a,evalValue);
            if(evalValue >= beta) break;
            first=false;

            if(alpha < evalValue) {
                alpha = evalValue;
                result = validCell;
            }
        }

        return result;
    }

private:
    CellState myColor, enemyColor;
    const i32 DEPTH_LEVEL;
    using BoardPair = std::pair<std::pair<u64, u64>, CellState>;
    using EvalPair = std::pair<i64, i64>;
    std::map<BoardPair, EvalPair> hash;
    EndGameEval eval;
    std::mt19937 mt;

    BoardPair makeBoardPair(const OthelloBoard<CellState::BLACK> &board1, const OthelloBoard<CellState::WHITE> &board2) noexcept {
        return BoardPair(std::make_pair(board1.getBitBoard(), board2.getBitBoard()),CellState::BLACK);
    }
    BoardPair makeBoardPair(const OthelloBoard<CellState::WHITE> &board1, const OthelloBoard<CellState::BLACK> &board2) noexcept {
        return BoardPair(std::make_pair(board1.getBitBoard(), board2.getBitBoard()),CellState::WHITE);
    }

    void updateHash(const BoardPair &boardPair,const EvalPair &evalPair) noexcept {
        if(hash.size() > (1<<22)) return;
        if(hash.count(boardPair)) {
            auto pair = hash[boardPair];
            pair.first = std::max(pair.first, evalPair.first);
            pair.second = std::min(pair.second, evalPair.second);
            hash[boardPair] = pair;

            assert(pair.first<=pair.second);
        }
        else {
            hash[boardPair]=evalPair;
        }
    }

    template<CellState Color1, CellState Color2>
    i64 dfs(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2, i32 depth, i64 alpha, i64 beta) noexcept {

        assert(alpha <= beta);
        static_assert(Color1 != Color2,"");

        if(isFinished(board1, board2) || depth == 0) {
            i64 result;
            if(Color1 == myColor) result = eval(board1,board2);
            else result = -eval(board2,board1);
            return result;
        }

        if(hash.count(makeBoardPair(board1, board2))) {
            auto evalPair = hash[makeBoardPair(board1, board2)];
            auto lower = evalPair.first;
            auto upper = evalPair.second;
            if(upper <= alpha) return upper;
            if(lower >= beta) return lower;
            if(upper == lower) return upper;

            alpha = std::max(alpha, lower);
            beta = std::min(beta, upper);
        }
        Cells validCells;
        if(false&&DEPTH_LEVEL-depth < 4) {
            validCells = makeOrderedValidCells(board1, board2);
        }
        else {
            validCells = makeValidCells(board1, board2);
        }
        std::shuffle(validCells.begin(), validCells.end(),mt);

        if(!validCells.size()) {
            return -dfs(board2, board1, depth-1, -beta, -alpha);
        }
        i64 score;
        i64 maxScore = minValue<i64>();
        i64 a=alpha;
        bool first=true;

        for(const auto &validCell : validCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;
            putStone(&boardTmp1, &boardTmp2, validCell.first, validCell.second);
            if(first) {
                score = -dfs(boardTmp2, boardTmp1, depth-1, -beta, -a);
            }
            else {
                score = -dfs(boardTmp2, boardTmp1, depth-1, -a-1, -a);
                if(score>a && score<beta && depth>1) score = -dfs(boardTmp2, boardTmp1, depth-1, -beta, -score);
            }

            if(score>=beta) {
                updateHash(makeBoardPair(board1, board2),EvalPair(score, maxValue<i64>()));
                return score;
            }
            if(score > maxScore) {
                a=std::max(a,score);
                maxScore = score;
                if(score > alpha) first=false;
            }
        }
        if(maxScore > alpha) 
            updateHash(makeBoardPair(board1, board2),EvalPair(maxScore,maxScore));
        else 
            updateHash(makeBoardPair(board1, board2),EvalPair(minValue<i64>(),maxScore));

        return maxScore;
    }

    template<CellState Color1, CellState Color2>
    Cells makeOrderedValidCells(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) noexcept {
        i64 alpha = minValue<i64>();
        i64 beta = maxValue<i64>();
        OrderedCells orderedValidCells;

        const auto validCells = makeValidCells(board1, board2);
        for(const auto &validCell : validCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;
            putStone(&boardTmp1, &boardTmp2, validCell.first, validCell.second);
            i64 evalValue = -negaAlpha(boardTmp2, boardTmp1, 4, -beta, -alpha);
            orderedValidCells.emplace_back(evalValue, validCell);
        }

        std::sort(orderedValidCells.begin(), orderedValidCells.end(), 
                [](const std::pair<i64, CellType> &cell1, const std::pair<i64, CellType> &cell2) {
                    return cell1.first > cell2.first;
                });
        Cells result;
        for(const auto &orderedValidCell : orderedValidCells) {
            result.emplace_back(orderedValidCell.second);
        }

        return std::move(result);
    }

    template<CellState Color1, CellState Color2>
    i64 negaAlpha(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2, i32 depth, i64 alpha, i64 beta) noexcept {
        assert(alpha <= beta);
        static_assert(Color1 != Color2,"");

        if(isFinished(board1, board2) || depth == 0) {
            i64 result;
            if(Color1 == myColor) result = eval(board1,board2);
            else result = -eval(board2,board1);
            return result;
        }
        auto validCells = makeValidCells(board1, board2);
        if(!validCells.size()) {
            return -negaAlpha(board2, board1, depth-1, -beta, -alpha);
        }
        for(const auto &validCell : validCells) {
            OthelloBoard<Color1> boardTmp1 = board1;
            OthelloBoard<Color2> boardTmp2 = board2;
            putStone(&boardTmp1, &boardTmp2, validCell.first, validCell.second);
            alpha = std::max(alpha, -negaAlpha(boardTmp2, boardTmp1, depth-1, -beta, -alpha));
            if(alpha >= beta) {
                return alpha;
            }
        }

        return alpha;
    }
};

} // end OthelloAI

#endif


