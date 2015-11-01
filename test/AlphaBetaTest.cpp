#include "../AlphaBeta.hpp"
#include "gtest/gtest.h"

class AlphaBetaTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};
namespace OthelloAI {
class Eval{
public:
    template<CellState Color1, CellState Color2>
    i64 operator()(const OthelloBoard<Color1> &board1, const OthelloBoard<Color2> &board2) {
        i64 result = countStone(board1);

        return result;
    }
};
}

TEST_F(AlphaBetaTest, EndGameTest) {
    OthelloAI::AlphaBeta<OthelloAI::Eval> ai(OthelloAI::CellState::WHITE, OthelloAI::CellState::BLACK, 20);
    auto blackBoard = OthelloAI::BlackBoard(0x3eb4e3af949d9820);
    auto whiteBoard = OthelloAI::WhiteBoard(0x00081c506b62240e);
    auto cell = ai(whiteBoard, blackBoard);

    ASSERT_EQ(cell == OthelloAI::CellType(1,1) || cell == OthelloAI::CellType(6,6), true);

    blackBoard = OthelloAI::BlackBoard(0x0076464a32305888);
    whiteBoard = OthelloAI::WhiteBoard(0xff89b9b5cdcf2534);
    cell = ai(whiteBoard, blackBoard);

    ASSERT_EQ(cell == OthelloAI::CellType(0,6), true);

}
    
