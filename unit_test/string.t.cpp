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
	EXPECT_EQ(0, string.length());
	EXPECT_TRUE(string.empty());
	EXPECT_EQ("", stream.str());
}

TEST(StringTest, CharConstructor)
{
	// GIVEN
	std::ostringstream stream;

	// WHEN
	String string{ 'a' };
	stream << string;

	// THEN
	EXPECT_FALSE(string.empty());
	EXPECT_EQ(1, string.length());
	EXPECT_EQ("a", stream.str());
}

TEST(StringTest, CStringConstructor)
{
	// GIVEN
	std::ostringstream stream;

	// WHEN
	String string{ "test" };
	stream << string;

	// THEN
	EXPECT_FALSE(string.empty());
	EXPECT_EQ(4, string.length());
	EXPECT_EQ("test", stream.str());
}

TEST(StringTest, CopyAssignment)
{
	// GIVEN
	String overridenString{ "test" };
	String overridingString{ "test2" };

	// WHEN
	overridenString = overridingString;

	// THEN
	EXPECT_EQ(5, overridenString.length());
	EXPECT_EQ(5, overridingString.length());
	EXPECT_EQ("test2", overridenString, "test2");
	EXPECT_EQ(overridenString, overridingString);
}

TEST(StringTest, CopyConstructor)
{
	// GIVEN
	String overridingString{ "test2" };

	// WHEN
	String newString = overridingString;

	// THEN
	EXPECT_EQ(5, newString.length());
	EXPECT_EQ(5, overridingString.length());
	EXPECT_EQ(newString, overridingString);
	EXPECT_EQ("test2", newString);
}

TEST(StringTest, MoveAssignment)
{
	// GIVEN
	String overridenString{ "test" };
	String overridingString{ "test2" };

	// WHEN
	overridenString = std::move(overridingString);

	// THEN
	EXPECT_EQ(5, overridenString.length());
	EXPECT_EQ("test2", overridenString);
}

TEST(StringTest, MoveConstructor)
{
	// GIVEN
	String overridingString{ "test2" };

	// WHEN
	String newString = std::move(overridingString);

	// THEN
	EXPECT_EQ(5, newString.length());
	EXPECT_EQ("test2", newString);
}

TEST(StringTest, Addition)
{
	// GIVEN
	String stringA{ "test" };
	String stringB{ "test2" };

	// WHEN
	String newString = stringA + stringB;

	// THEN
	EXPECT_EQ("testtest2", newString);
	EXPECT_EQ("test", stringA);
	EXPECT_EQ("test2", stringB);
}

TEST(StringTest, EqualityOperator)
{
	// GIVEN
	String stringA{ "test" };
	String stringB{ "test2" };
	String stringC{ "test" };

	// WHEN

	// THEN
	EXPECT_FALSE(stringA == stringB);
	EXPECT_TRUE(stringA != stringB);
	EXPECT_TRUE(stringA == stringC);
	EXPECT_FALSE(stringA != stringC);
}

TEST(StringTest, IncrementOperator)
{
	// GIVEN
	String stringA{ "test" };
	String stringB{ "test2" };

	// WHEN
	stringA += stringB;

	// THEN
	EXPECT_EQ("testtest2", stringA);
	EXPECT_EQ("test2", stringB);
}

TEST(StringTest, NonConstIterators)
{
	// GIVEN
	String string{ "abcd" };

	// WHEN
	std::reverse(string.begin(), string.end());

	// THEN
	EXPECT_EQ("dcba", string);
}

TEST(StringTest, ConstIterators)
{
	// GIVEN
	String string{ "test" };

	// WHEN
	std::ostringstream stream;
	std::for_each(string.cbegin(), string.cend(), [&stream](const auto& c) {stream << c; });

	// THEN
	EXPECT_EQ("test", stream.str());
}

TEST(StringTest, stoiPositiveNumber)
{
	// GIVEN
	String string{ "123" };

	// WHEN
	const int outputNumber = stoi(string);

	// THEN
	EXPECT_EQ(123, outputNumber);
}

TEST(StringTest, stoiNegativeNumber)
{
	// GIVEN
	String string{ "-123" };

	// WHEN
	const int outputNumber = stoi(string);

	// THEN
	EXPECT_EQ(-123, outputNumber);
}

TEST(StringTest, stoiInvalidNumberThrows)
{
	// GIVEN
	String string{ "abc" };

	// WHEN

	// THEN
	EXPECT_THROW(stoi(string), std::invalid_argument);
}

TEST(StringTest, stoiIntegerOverflowThrows)
{
	// GIVEN
	String string{ "12345678901234567890" };

	// WHEN

	// THEN
	EXPECT_THROW(stoi(string), std::runtime_error);
}

TEST(StringTest, stoiNegativeButNoNumbersThrows)
{
	// GIVEN
	String string{ "-" };

	// WHEN

	// THEN
	EXPECT_THROW(stoi(string), std::invalid_argument);
}

TEST(StringTest, stoiEmptyStringThrows)
{
	// GIVEN
	String string{ "" };

	// WHEN

	// THEN
	EXPECT_THROW(stoi(string), std::invalid_argument);
}

TEST(StringTest, toStringPositiveNumber)
{
	// GIVEN
	int num = 123;

	// WHEN
	String res = toString(num);

	// THEN
	EXPECT_EQ("123", res);
}

TEST(StringTest, toStringNegativeNumber)
{
	// GIVEN
	int num = -123;

	// WHEN
	String res = toString(num);

	// THEN
	EXPECT_EQ("-123", res);
}

TEST(StringTest, toStringZero)
{
	// GIVEN
	int num = 0;

	// WHEN
	String res = toString(num);

	// THEN
	EXPECT_EQ("0", res);
}

TEST(StringTest, findHasSubstring)
{
	// GIVEN
	String haystack{ "abcd" };
	String needle{ "cd" };

	// WHEN
	int index = haystack.find(needle);

	// THEN
	EXPECT_EQ(2, index);
}

TEST(StringTest, findNoSubstring)
{
	// GIVEN
	String haystack{ "abcd" };
	String needle{ "ce" };

	// WHEN
	int index = haystack.find(needle);

	// THEN
	EXPECT_EQ(-1, index);
}