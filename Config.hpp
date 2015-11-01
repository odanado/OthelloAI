#ifndef INCLUDE_OTHELLOAI_CONFIG_HPP
#define INCLUDE_OTHELLOAI_CONFIG_HPP

#include <cstdint>
#include <vector>
#include <utility>
#include <limits>

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

template<typename T>
constexpr T maxValue = std::numeric_limits<T>::max() >> 4;
template<typename T>
constexpr T minValue = -maxValue<T>;

using CellType = std::pair<u64, u64>;
using Cells = std::vector<CellType>;

} // end OthelloAI

#endif

