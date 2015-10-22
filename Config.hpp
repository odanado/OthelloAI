#ifndef INCLUDE_OTHELLOAI_CONFIG_HPP
#define INCLUDE_OTHELLOAI_CONFIG_HPP

#include <cstdint>
#include <vector>
#include <utility>

namespace OthelloAI {

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using i32 = std::int32_t;

enum CellState {
    NONE,
    BLACK,
    WHITE
};

using CellType = std::pair<u64, u64>;
using Cells = std::vector<CellType>;

} // end OthelloAI

#endif

