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
	std::string* strPtr = new std::string("abc");
	SharedPtr<std::string> ptr{strPtr};

	// WHEN

	// THEN
	EXPECT_EQ(strPtr, ptr.get());
	EXPECT_EQ("abc", *ptr);
	EXPECT_EQ(3, ptr->length());
}