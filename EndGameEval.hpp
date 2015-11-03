#ifndef INCLUDE_OTHELLOAI_ENDGAME_EVAL_HPP
#define INCLUDE_OTHELLOAI_ENDGAME_EVAL_HPP

#include "Config.hpp"
#include "OthelloBoard.hpp"

namespace OthelloAI {

/**
 * 終盤の評価関数
 */
class EndGameEval {
public:
    template<CellState Color1, CellState Color2>
    i64 operator()(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) const noexcept {
        return countStone(board1) - countStone(board2);
    }
};
} // end OthelloAI

#endif

