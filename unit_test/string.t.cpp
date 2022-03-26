#include "pch.h"

#include "../implementations/string.cpp"

using namespace implementations;

TEST(StringTest, DefaultConstructor)
{
	// GIVEN
	std::ostringstream stream;

	// WHEN
	String string;
	stream << string;

	// THEN
	EXPECT_EQ(string.length(), 0);
	EXPECT_TRUE(string.empty());
	EXPECT_EQ(stream.str(), "");
}

TEST(StringTest, PrimitiveConstructor)
{
	// GIVEN
	std::ostringstream stream;

	// WHEN
	String string{ "test" };
	stream << string;

	// THEN
	EXPECT_FALSE(string.empty());
	EXPECT_EQ(string.length(), 4);
	EXPECT_EQ(stream.str(), "test");
}

TEST(StringTest, CopyConstructor)
{
	// GIVEN
	String overridenString{ "test" };
	String overridingString{ "test2" };

	// WHEN
	overridenString = overridingString;

	// THEN
	EXPECT_EQ(overridenString.length(), 5);
	EXPECT_EQ(overridingString.length(), 5);
	EXPECT_EQ(overridenString, "test2");
	EXPECT_EQ(overridenString, overridingString);
}

TEST(StringTest, CopyAssignment)
{
	// GIVEN
	String overridingString{ "test2" };

	// WHEN
	String newString = overridingString;

	// THEN
	EXPECT_EQ(newString.length(), 5);
	EXPECT_EQ(overridingString.length(), 5);
	EXPECT_EQ(newString, overridingString);
	EXPECT_EQ(newString, "test2");
}