#ifndef INCLUDE_MINMAXAI_HPP
#define INCLUDE_MINMAXAI_HPP

#include "Config.hpp"
#include "OthelloBoard.hpp"

namespace OthelloAI {

class MiniMaxAI {
public:
    MiniMaxAI(CellState myColor, CellState enemyColor);

    template<CellState Color1, CellState Color2>
    CellType operator()(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) noexcept;

private:
    CellState myColor, enemyColor;
    template<CellState Color1, CellState Color2>
    i64 dfs(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2, u32 depth) noexcept;
    template<CellState Color1, CellState Color2>
    i64 eval(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) noexcept;
    
    static const u32 DEPTH_LEVAL;
};

} // end OhelloAI

#endif
