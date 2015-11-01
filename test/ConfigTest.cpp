#include "../Config.hpp"
#include "gtest/gtest.h"

class ConfigTest : public ::testing::Test {
protected:
    virtual void SetUp() {
    }
};

TEST_F(ConfigTest, ConstantValueTest) {
    ASSERT_EQ(OthelloAI::maxValue<OthelloAI::i64>(), -OthelloAI::minValue<OthelloAI::i64>());
}

