#include "../OthelloUtils.hpp"
#include "gtest/gtest.h"

class OthelloUtilsTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(OthelloUtilsTest, cellToInt) {
    using namespace OthelloAI;
    EXPECT_EQ(0, cellToInt(CellType(0,0)));
    EXPECT_EQ(10, cellToInt(CellType(1,2)));
    EXPECT_EQ(30, cellToInt(CellType(3,6)));
    EXPECT_EQ(55, cellToInt(CellType(6,7)));
    EXPECT_EQ(63, cellToInt(CellType(7,7)));
}
