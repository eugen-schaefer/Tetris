#include "gtest/gtest.h"
#include "../src/my_function.h"

TEST(TestSuiteName, DemoGTest) {
    EXPECT_TRUE(true);
}

TEST(TestSuiteName, TestMyFunction) {
    int inputValue{10};
    int returnValue = DoubleTheValue(inputValue);
    int expectedValue{20};
    EXPECT_EQ(expectedValue, returnValue);
}