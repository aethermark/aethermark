#include <gtest/gtest.h>
#include "aethermark/aethermark.hpp"

TEST(GreetTest, Basic)
{
    EXPECT_EQ(aethermark::greet("World"), "Hello, World from Aethermark!");
}
