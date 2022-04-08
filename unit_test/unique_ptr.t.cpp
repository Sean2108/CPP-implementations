#include "pch.h"

#include "../implementations/unique_ptr.cpp"
#include <string>

using namespace implementations;

TEST(UniquePtrTest, DefaultConstructor) {
	// GIVEN
	UniquePtr<std::string> ptr;

	// WHEN
	std::string* underlying = ptr.get();

	// THEN
	EXPECT_EQ(nullptr, underlying);
}

TEST(UniquePtrTest, MoveConstructor) {
	// GIVEN
	UniquePtr<std::string> ptr = makeUnique<std::string>("abc");

	// WHEN
	UniquePtr<std::string> ptr2(std::move(ptr));

	// THEN
	EXPECT_EQ("abc", *ptr2);
	EXPECT_FALSE(ptr);
}

TEST(UniquePtrTest, MoveAssignment) {
	// GIVEN
	UniquePtr<std::string> ptr = makeUnique<std::string>("abc");
	UniquePtr<std::string> ptr2 = makeUnique<std::string>("def");

	// WHEN
	ptr2 = std::move(ptr);

	// THEN
	EXPECT_EQ("abc", *ptr2);
	EXPECT_FALSE(ptr);
}

TEST(UniquePtrTest, Reset) {
	// GIVEN
	UniquePtr<std::string> ptr = makeUnique<std::string>("abcdef");

	// WHEN
	const size_t len = ptr->length();
	ptr.reset();

	// THEN
	EXPECT_EQ(6, len);
	EXPECT_FALSE(ptr);
}