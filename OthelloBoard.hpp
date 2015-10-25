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
    friend bool reverseStone(OthelloBoard<myColor> *myBoard, OthelloBoard<enemyColor> *enemyBoard, u64 y, u64 x, int dy,int dx) noexcept;

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

template<CellState myColor, CellState enemyColor>
bool reverseStone(OthelloBoard<myColor> *myBoard, OthelloBoard<enemyColor> *enemyBoard, u64 y, u64 x, int dy,int dx) noexcept {
    assert(myColor != enemyColor);

    // 範囲外
    if(y > 7 || x > 7) return false;
    // 隣接する石を移動したけど石のないところに来た
    if(!myBoard->existStone(y,x) && !enemyBoard->existStone(y,x)) return false;
    // 自分の石を見つけた
    if(myBoard->existStone(y,x)) return true;

    bool reversible = reverseStone(myBoard, enemyBoard, y+dy, x+dx, dy, dx);

    // 反転可能な手なら反転する
    if(reversible) {
        myBoard->setStone(y,x);
        enemyBoard->clearStone(y,x);
    }

    return reversible;
}

template<CellState myColor, CellState enemyColor>
bool putStone(OthelloBoard<myColor> *myBoard, OthelloBoard<enemyColor> *enemyBoard, u64 y, u64 x) noexcept {
    bool reversible = false;
    assert(myColor != enemyColor);
    // 既に石がある
    if(myBoard->existStone(y,x) || enemyBoard->existStone(y,x)) {
        return false;
    }

    for(int dy = -1; dy <= 1; ++dy) {
        if(y + dy > 7) continue;
        for(int dx = -1; dx <= 1; ++dx) {
            if(x + dx > 7) continue;

            // 相手の石がある かつ うまくリバースできた
            if(enemyBoard->existStone(y+dy, x+dx) && reverseStone(myBoard, enemyBoard, y+dy, x+dx, dy, dx)) {
                myBoard->setStone(y,x);
                reversible = true;
            }
        }
    }

    return reversible;
}

template<CellState myColor, CellState enemyColor>
Cells makeVaildCells(const OthelloBoard<myColor> &myBoard, const OthelloBoard<enemyColor> &enemyBoard) noexcept {
    Cells cells;
    for(u64 y = 0; y < 8; ++y) {
        for(u64 x = 0; x < 8; ++x) {
            auto myBoardTmp = myBoard;
            auto enemyBoardTmp = enemyBoard;

            if(putStone(&myBoardTmp, &enemyBoardTmp, y, x)) {
                cells.emplace_back(y,x);
            }
        }
    }

    return std::move(cells);
}

template<CellState myColor, CellState enemyColor>
bool isFinished(const OthelloBoard<myColor> &myBoard, const OthelloBoard<enemyColor> &enemyBoard) noexcept {
    return makeVaildCells(myBoard, enemyBoard).empty() && makeVaildCells(enemyBoard, myBoard).empty();
}

} // end OthelloAI

#endif

