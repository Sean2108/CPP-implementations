#include "pch.h"

#include "../implementations/string.cpp"

using namespace implementations;

TEST(StringTest, DefaultConstructor)
{
    String string;

    SUCCEED();
}