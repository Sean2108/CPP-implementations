#include "pch.h"

#include "../implementations/shared_ptr.cpp"
#include <string>

using namespace implementations;

TEST(SharedPtrTest, DefaultConstructor) {
	// GIVEN
	SharedPtr<std::string> ptr;

	// WHEN

	// THEN
	EXPECT_EQ(nullptr, ptr.get());
}

TEST(SharedPtrTest, PtrConstructor) {
	// GIVEN

	// WHEN
	SharedPtr<std::string> ptr = makeShared<std::string>("abc");

	// THEN
	EXPECT_EQ("abc", *ptr);
	EXPECT_EQ(3, ptr->length());
}

TEST(SharedPtrTest, CopyConstructor) {
	// GIVEN
	std::string* strPtr = new std::string("abc");
	SharedPtr<std::string> ptr{ strPtr };

	// WHEN
	SharedPtr<std::string> copiedPtr{ ptr };

	// THEN
	EXPECT_EQ(*ptr, *copiedPtr);
	EXPECT_EQ(2, ptr.useCount());
	EXPECT_EQ(2, copiedPtr.useCount());
}

TEST(SharedPtrTest, CopyAssignment) {
	// GIVEN
	auto ptrToA = makeShared<std::string>("abc");
	auto ptrToB = makeShared<std::string>("def");
	SharedPtr<std::string> ptrToB2{ ptrToB };
	SharedPtr<std::string> ptrToB3{ ptrToB };
	SharedPtr<std::string> ptrToB4{ ptrToB };
	SharedPtr<std::string> copiedPtr{ ptrToB };

	// WHEN
	EXPECT_EQ(5, ptrToB.useCount());
	EXPECT_EQ(1, ptrToA.useCount());
	copiedPtr = ptrToA;

	// THEN
	EXPECT_EQ(*ptrToA, "abc");
	EXPECT_EQ(*ptrToB, "def");
	EXPECT_EQ(*copiedPtr, "abc");
	EXPECT_EQ(2, ptrToA.useCount());
	EXPECT_EQ(4, ptrToB.useCount());
	EXPECT_EQ(2, copiedPtr.useCount());
}

TEST(SharedPtrTest, MoveConstructor) {
	// GIVEN
	auto ptr = makeShared<std::string>("abc");

	// WHEN
	SharedPtr<std::string> ptr2{ std::move(ptr) };

	// THEN
	EXPECT_FALSE(ptr);
	EXPECT_EQ(1, ptr2.useCount());
	EXPECT_EQ("abc", *ptr2);
}

TEST(SharedPtrTest, MoveAssignment) {
	// GIVEN
	auto ptr = makeShared<std::string>("abc");
	auto ptr2 = makeShared<std::string>("def");

	// WHEN
	ptr2 = std::move(ptr);

	// THEN
	EXPECT_FALSE(ptr);
	EXPECT_EQ(1, ptr2.useCount());
	EXPECT_EQ("abc", *ptr2);
}

TEST(SharedPtrTest, Reset) {
	// GIVEN
	auto ptr = makeShared<std::string>("abc");

	// WHEN
	ptr.reset();

	// THEN
	EXPECT_FALSE(ptr);
}