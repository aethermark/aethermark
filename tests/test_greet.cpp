#include <gtest/gtest.h>
#include "quantamark/quantamark.hpp"

TEST(GreetTest, Basic) {
    EXPECT_EQ(quantamark::greet("World"), "Hello, World from Quantamark!");
}
