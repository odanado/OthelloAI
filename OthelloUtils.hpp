#ifndef INCLUDE_OTHELLOAI_OTHELLO_UTILS_HPP
#define INCLUDE_OTHELLOAI_OTHELLO_UTILS_HPP

#include "Config.hpp"

namespace OthelloAI {

    inline u64 cellToInt(const CellType &cell) noexcept { 
        return (cell.first<<3)+cell.second; 
    }

} // end OthelloAI

#endif

