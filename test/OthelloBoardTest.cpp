#include "../OthelloBoard.hpp"
#include <gtest/gtest.h>

class OthelloBoardTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
    OthelloAI::BlackBoard blackBoard;
    OthelloAI::WhiteBoard whiteBoard;
};


TEST_F(OthelloBoardTest, switchCellState) {
    ASSERT_EQ(OthelloAI::CellState::BLACK, OthelloAI::switchCellState(OthelloAI::CellState::WHITE));
    ASSERT_EQ(OthelloAI::CellState::WHITE, OthelloAI::switchCellState(OthelloAI::CellState::BLACK));
}

TEST_F(OthelloBoardTest, getColor) {
    ASSERT_EQ(OthelloAI::BlackBoard::getColor(), OthelloAI::CellState::BLACK);
    ASSERT_EQ(OthelloAI::WhiteBoard::getColor(), OthelloAI::CellState::WHITE);
    ASSERT_EQ(blackBoard.getColor(), OthelloAI::CellState::BLACK);
    ASSERT_EQ(whiteBoard.getColor(), OthelloAI::CellState::WHITE);
}


TEST_F(OthelloBoardTest, OthelloBoard) {
    ASSERT_EQ(blackBoard.existStone(4,3),true);
    ASSERT_EQ(blackBoard.existStone(3,4),true);

    ASSERT_EQ(whiteBoard.existStone(3,3),true);
    ASSERT_EQ(whiteBoard.existStone(4,4),true);

    ASSERT_EQ(blackBoard == blackBoard, true);
    ASSERT_EQ(whiteBoard == whiteBoard, true);
    
    // http://www.cc.u-tokyo.ac.jp/support/press/news/VOL13/No5/Rennsai201109.pdf の40
    auto board = OthelloAI::BlackBoard(0x10783f3b737e79);
    board.existStone(0,0);
    board.existStone(2,0);
    board.existStone(3,0);
    board.existStone(4,0);

    board.existStone(1,1);
    board.existStone(2,1);
    board.existStone(3,1);
    board.existStone(4,1);

    board.existStone(1,2);
    board.existStone(4,2);

    board.existStone(0,3);
    board.existStone(1,3);
    board.existStone(3,3);
    board.existStone(4,3);
    board.existStone(5,3);

    board.existStone(0,4);
    board.existStone(1,4);
    board.existStone(2,4);
    board.existStone(3,4);
    board.existStone(4,4);
    board.existStone(5,4);
    board.existStone(6,4);

    board.existStone(0,5);
    board.existStone(1,5);
    board.existStone(2,5);
    board.existStone(3,5);
    board.existStone(4,5);
    board.existStone(5,5);

    board.existStone(0,6);
    board.existStone(1,6);
    board.existStone(2,6);
    board.existStone(5,6);

    ASSERT_EQ(board.popCount(), OthelloAI::u64(32));

    board = OthelloAI::BlackBoard(0x8080c0c48c8080);

    board.existStone(0,7);

    board.existStone(1,7);

    board.existStone(2,2);
    board.existStone(2,3);
    board.existStone(2,7);

    board.existStone(3,2);
    board.existStone(3,6);
    board.existStone(3,7);

    board.existStone(4,6);
    board.existStone(4,7);

    board.existStone(5,7);

    board.existStone(6,7);

    ASSERT_EQ(board.popCount(), OthelloAI::u64(12));
}

TEST_F(OthelloBoardTest, putStone) {
    // http://www.cc.u-tokyo.ac.jp/support/press/news/VOL13/No5/Rennsai201109.pdf の40
    auto blackBoard = OthelloAI::BlackBoard(0x8080c0c48c8080);
    auto whiteBoard = OthelloAI::WhiteBoard(0x10783f3b737e79);

    putStone(&blackBoard, &whiteBoard, 5, 2);

    ASSERT_EQ(blackBoard.getBitBoard() & whiteBoard.getBitBoard(), OthelloAI::u64(0));
    ASSERT_EQ(blackBoard.getBitBoard(), OthelloAI::u64(0x0080fcccd4acc080));
    ASSERT_EQ(whiteBoard.getBitBoard(), OthelloAI::u64(0x001000332b533e79));

    // 変化しない手
    putStone(&blackBoard, &whiteBoard, 7, 7);
    ASSERT_EQ(blackBoard.getBitBoard(), OthelloAI::u64(0x0080fcccd4acc080));
    ASSERT_EQ(whiteBoard.getBitBoard(), OthelloAI::u64(0x001000332b533e79));

    /*
     * xxxxxxxx
     * xoooooox
     * xoooooox
     * xoo.ooox
     * xoooooox
     * xoooooox
     * xoooooox
     * xxxxxxxx
     */
    blackBoard = OthelloAI::BlackBoard(0xff818181818181ff);
    whiteBoard = OthelloAI::WhiteBoard(0x007e7e7e767e7e00);

    putStone(&blackBoard, &whiteBoard, 3, 3);

    ASSERT_EQ(blackBoard.getBitBoard() & whiteBoard.getBitBoard(), OthelloAI::u64(0));
    ASSERT_EQ(blackBoard.getBitBoard(), OthelloAI::u64(0xffc9ab9dff9dabff));
    ASSERT_EQ(whiteBoard.getBitBoard(), OthelloAI::u64(0x0036546200625400));
}

TEST_F(OthelloBoardTest, makeVaildCells) {
    // http://www.cc.u-tokyo.ac.jp/support/press/news/VOL13/No5/Rennsai201109.pdf の40
    auto blackBoard = OthelloAI::BlackBoard(0x8080c0c48c8080);
    auto whiteBoard = OthelloAI::WhiteBoard(0x10783f3b737e79);

    auto vaildCells =  makeVaildCells(blackBoard, whiteBoard);
    ASSERT_EQ(vaildCells.size(), std::size_t(10));
    ASSERT_EQ(vaildCells[0], OthelloAI::CellType(0,1));
    ASSERT_EQ(vaildCells[1], OthelloAI::CellType(0,2));
    ASSERT_EQ(vaildCells[2], OthelloAI::CellType(1,0));
    ASSERT_EQ(vaildCells[3], OthelloAI::CellType(5,0));
    ASSERT_EQ(vaildCells[4], OthelloAI::CellType(5,2));
    ASSERT_EQ(vaildCells[5], OthelloAI::CellType(6,2));
    ASSERT_EQ(vaildCells[6], OthelloAI::CellType(6,3));
    ASSERT_EQ(vaildCells[7], OthelloAI::CellType(6,5));
    ASSERT_EQ(vaildCells[8], OthelloAI::CellType(6,6));
    ASSERT_EQ(vaildCells[9], OthelloAI::CellType(7,3));

    vaildCells = makeVaildCells(whiteBoard, blackBoard);
    ASSERT_EQ(vaildCells.size(), std::size_t(0));
}

TEST_F(OthelloBoardTest, isFinished) {
    auto blackBoard = OthelloAI::BlackBoard();
    auto whiteBoard = OthelloAI::WhiteBoard();

    ASSERT_EQ(isFinished(blackBoard, whiteBoard), false);

    blackBoard = OthelloAI::BlackBoard(0xffefff5f7f3f1f0f);
    whiteBoard = OthelloAI::WhiteBoard(0x001000a080c0e0f0);
    ASSERT_EQ(isFinished(blackBoard, whiteBoard), true);
}

