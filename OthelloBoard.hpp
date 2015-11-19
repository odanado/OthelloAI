#ifndef INCLUDE_OTHELLOAI_OTHELLO_BOARD_HPP
#define INCLUDE_OTHELLOAI_OTHELLO_BOARD_HPP

#include "Config.hpp"
#include <cassert>

namespace OthelloAI {

namespace detail {
template<CellState>
constexpr u64 initCellState();
}

template<CellState Tag>
class OthelloBoard {
private:
    u64 bitBoard;
    void setStone(u64 y, u64 x) noexcept {
        assert(0 <= y && y < 8);
        assert(0 <= x && x < 8);
        bitBoard |= (1ULL << ((y << 3) + x));
    }

    void clearStone(u64 y, u64 x) noexcept {
        assert(0 <= y && y < 8);
        assert(0 <= x && x < 8);
        bitBoard &= (0xFFFFFFFFFFFFFFFF ^ (1ULL << ((y << 3) + x)));
    }

public:
    OthelloBoard(u64 bitBoard = detail::initCellState<Tag>()) : bitBoard(bitBoard) {}
    OthelloBoard<Tag>& operator=(const OthelloBoard<Tag> &rhs) {
        bitBoard = rhs.bitBoard;
        return *this;
    }
    OthelloBoard(const OthelloBoard<Tag> &rhs) {
        bitBoard = rhs.bitBoard;
    }

    static CellState getColor() noexcept {
        return Tag;
    }

    u64 getBitBoard() const noexcept {
        return bitBoard;
    }

    u64 countStone() const noexcept {
        return __builtin_popcount(bitBoard) + __builtin_popcount(bitBoard >> 32);
    }

    bool existStone(u64 y, u64 x) const noexcept {
        assert(0 <= y && y < 8);
        assert(0 <= x && x < 8);
        return (bitBoard >> ((y << 3) + x)) & 1;
    }

    bool operator==(const OthelloBoard<Tag> &rhs) const {
        return bitBoard == rhs.bitBoard;
    }

    template<CellState myColor>
    friend u64 countStone(const OthelloBoard<myColor> &myBoard) noexcept;
    template<CellState myColor>
    friend bool existStone(const OthelloBoard<myColor> &myBoard, u64 y, u64 x) noexcept;

    template<CellState myColor, CellState enemyColor>
    friend bool putStone(OthelloBoard<myColor> *myBoard, OthelloBoard<enemyColor> *enemyBoard, u64 y, u64 x) noexcept;

};

using BlackBoard = OthelloBoard<CellState::BLACK>;
using WhiteBoard = OthelloBoard<CellState::WHITE>;

constexpr CellState switchCellState(CellState cellState) {
    return cellState == CellState::BLACK ? 
        CellState::WHITE: 
        CellState::BLACK;
}

namespace detail {
template<>
constexpr u64 initCellState<CellState::BLACK>() {
    return 0x810000000;
}
template<>
constexpr u64 initCellState<CellState::WHITE>() {
    return 0x1008000000;
}
} // end detail

template<CellState myColor>
u64 countStone(const OthelloBoard<myColor> &myBoard) noexcept {
    return myBoard.countStone();
}
template<CellState myColor>
bool existStone(const OthelloBoard<myColor> &myBoard, u64 y, u64 x) noexcept {
    return myBoard.existStone(y,x);
}


inline u64 makeReverseBit(u64 black, u64 white, u64 pos) noexcept {
    u64 rev=0,tmp,mask;

    // 右
    tmp=0;
    mask = (pos >> 1) & 0x7f7f7f7f7f7f7f7f;
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask >> 1) & 0x7f7f7f7f7f7f7f7f;
    }
    if(mask & black) rev |= tmp;

    // 左
    tmp=0;
    mask = (pos << 1) & 0xfefefefefefefefe;
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask << 1) & 0xfefefefefefefefe;
    }
    if(mask & black) rev |= tmp;

    // 上
    tmp=0;
    mask = (pos << 8);
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask << 8);
    }
    if(mask & black) rev |= tmp;

    // 下
    tmp=0;
    mask = (pos >> 8);
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask >> 8);
    }
    if(mask & black) rev |= tmp;

    // 右上
    tmp=0;
    mask = (pos << 7) & 0x7f7f7f7f7f7f7f7f;
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask << 7) & 0x7f7f7f7f7f7f7f7f;
    }
    if(mask & black) rev |= tmp;

    // 左上
    tmp=0;
    mask = (pos << 9) & 0xfefefefefefefefe;
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask << 9) & 0xfefefefefefefefe;
    }
    if(mask & black) rev |= tmp;

    // 右下
    tmp=0;
    mask = (pos >> 9) & 0x7f7f7f7f7f7f7f7f;
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask >> 9) & 0x7f7f7f7f7f7f7f7f;
    }
    if(mask & black) rev |= tmp;
    
    // 左下
    tmp=0;
    mask = (pos >> 7) & 0xfefefefefefefefe;
    while(mask && (mask & white)) {
        tmp |= mask;
        mask = (mask >> 7) & 0xfefefefefefefefe;
    }
    if(mask & black) rev |= tmp;


    return rev;
}

template<CellState myColor, CellState enemyColor>
bool putStone(OthelloBoard<myColor> *myBoard, OthelloBoard<enemyColor> *enemyBoard, u64 y, u64 x) noexcept {
    assert(myColor != enemyColor);

    // 既に石がある
    if(myBoard->existStone(y,x) || enemyBoard->existStone(y,x)) {
        return false;
    }

    u64 pos = u64(1) << ((y<<3)+x);
    u64 rev = makeReverseBit(myBoard->bitBoard, enemyBoard->bitBoard, pos);
    if(rev == 0) 
        return false;

    myBoard->bitBoard ^= pos | rev;
    enemyBoard->bitBoard ^= rev;

    return true;
}

template<CellState myColor, CellState enemyColor>
bool putStone(OthelloBoard<myColor> *myBoard, OthelloBoard<enemyColor> *enemyBoard, const CellType &cell) noexcept {
    assert(myColor != enemyColor);
    return putStone(myBoard, enemyBoard, cell.first, cell.second);
}

inline u64 makeReversiblePos(u64 black, u64 white) noexcept {
    u64 blank = ~(black | white);
    u64 mobility=0,t,w;

    // 右
    w = white & 0x7e7e7e7e7e7e7e7e; 
    t = w & (black << 1);
    t |= w & (t << 1); t |= w & (t << 1); t |= w & (t << 1);
    t |= w & (t << 1); t |= w & (t << 1);
    mobility |= blank & (t << 1);

    // 左
    w = white & 0x7e7e7e7e7e7e7e7e; 
    t = w & (black >> 1);
    t |= w & (t >> 1); t |= w & (t >> 1); t |= w & (t >> 1);
    t |= w & (t >> 1); t |= w & (t >> 1);
    mobility |= blank & (t >> 1);

    // 上
    w = white & 0x00ffffffffffff00;
    t = w & (black << 8);
    t |= w & (t << 8); t |= w & (t << 8); t |= w & (t << 8);
    t |= w & (t << 8); t |= w & (t << 8);
    mobility |= blank & (t << 8);

    // 下
    w = white & 0x00ffffffffffff00;
    t = w & (black >> 8);
    t |= w & (t >> 8); t |= w & (t >> 8); t |= w & (t >> 8);
    t |= w & (t >> 8); t |= w & (t >> 8);
    mobility |= blank & (t >> 8);

    // 右上
    w = white & 0x007e7e7e7e7e7e00;
    t = w & (black << 7);
    t |= w & (t << 7); t |= w & (t << 7); t |= w & (t << 7);
    t |= w & (t << 7); t |= w & (t << 7);
    mobility |= blank & (t << 7);

    // 左上
    w = white & 0x007e7e7e7e7e7e00;
    t = w & (black << 9);
    t |= w & (t << 9); t |= w & (t << 9); t |= w & (t << 9);
    t |= w & (t << 9); t |= w & (t << 9);
    mobility |= blank & (t << 9);
    
    // 右下
    w = white & 0x007e7e7e7e7e7e00;
    t = w & (black >> 9);
    t |= w & (t >> 9); t |= w & (t >> 9); t |= w & (t >> 9);
    t |= w & (t >> 9); t |= w & (t >> 9);
    mobility |= blank & (t >> 9);

    // 左下
    w = white & 0x007e7e7e7e7e7e00;
    t = w & (black >> 7);
    t |= w & (t >> 7); t |= w & (t >> 7); t |= w & (t >> 7);
    t |= w & (t >> 7); t |= w & (t >> 7);
    mobility |= blank & (t >> 7);

    return mobility;
}

template<CellState myColor, CellState enemyColor>
Cells makeValidCells(const OthelloBoard<myColor> &myBoard, const OthelloBoard<enemyColor> &enemyBoard) noexcept {
    Cells cells;

    auto reversiblePos = makeReversiblePos(myBoard.getBitBoard(), enemyBoard.getBitBoard());
    for(u64 k=0;k<64;++k) {
        if(reversiblePos>>k&1) {
            cells.emplace_back(k>>3,k&7);
        }
    }

    return std::move(cells);
}

template<CellState myColor, CellState enemyColor>
bool isFinished(const OthelloBoard<myColor> &myBoard, const OthelloBoard<enemyColor> &enemyBoard) noexcept {
    return makeValidCells(myBoard, enemyBoard).empty() && makeValidCells(enemyBoard, myBoard).empty();
}

} // end OthelloAI

#endif

