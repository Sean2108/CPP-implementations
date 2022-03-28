#include "pch.h"

#include "../implementations/vector.cpp"

using namespace implementations;

TEST(VectorTest, DefaultConstructor) {
	// GIVEN
	Vector<int> vec;

	// WHEN

	// THEN
	EXPECT_EQ(0, vec.size());
	EXPECT_EQ(0, vec.capacity());
	EXPECT_TRUE(vec.empty());
}

TEST(VectorTest, SizeConstructor) {
	// GIVEN
	Vector<int> vec(5, 7);

	// WHEN

	// THEN
	EXPECT_EQ(5, vec.size());
	EXPECT_EQ(5, vec.capacity());
	ASSERT_FALSE(vec.empty());
	EXPECT_TRUE(std::all_of(vec.cbegin(), vec.cend(), [](const auto& item) {return item == 7; }));
}

TEST(VectorTest, InitializerListConstructor) {
	// GIVEN
	Vector<int> vec{ 1, 2, 3 };

	// WHEN

	// THEN
	EXPECT_EQ(3, vec.size());
	EXPECT_EQ(3, vec.capacity());
	ASSERT_FALSE(vec.empty());
	EXPECT_EQ(1, vec[0]);
	EXPECT_EQ(2, vec[1]);
	EXPECT_EQ(3, vec[2]);
	EXPECT_THROW(vec.at(3), std::out_of_range);
}

TEST(VectorTest, IteratorConstructor) {
	// GIVEN
	int input[3]{ 1, 2, 3 };
	Vector<int> vec(input, input + 3);

	// WHEN

	// THEN
	EXPECT_EQ(3, vec.size());
	EXPECT_EQ(3, vec.capacity());
	ASSERT_FALSE(vec.empty());
	EXPECT_EQ(1, vec[0]);
	EXPECT_EQ(2, vec[1]);
	EXPECT_EQ(3, vec[2]);
	vec[1] = 4;
	EXPECT_EQ(4, vec[1]);
}

TEST(VectorTest, CopyConstructor) {
	// GIVEN
	Vector<int> vec1 = { 1, 2, 3 };

	// WHEN
	Vector<int> vec2(vec1);

	// THEN
	EXPECT_EQ(3, vec2.size());
	EXPECT_EQ(vec2, vec1);
}

TEST(VectorTest, CopyAssignment) {
	// GIVEN
	Vector<int> vec1 = { 1, 2, 3 };
	Vector<int> vec2 = { 4, 5, 6 };

	// WHEN
	vec2 = vec1;

	// THEN
	EXPECT_EQ(vec2, vec1);
}

TEST(VectorTest, MoveConstructor) {
	// GIVEN
	Vector<int> vec1 = { 1, 2, 3 };
	Vector<int> vec2 = vec1;

	// WHEN
	Vector<int> vec3(std::move(vec1));

	// THEN
	EXPECT_EQ(3, vec2.size());
	EXPECT_EQ(vec3, vec2);
	EXPECT_TRUE(vec1.empty());
}

TEST(VectorTest, MoveAssignment) {
	// GIVEN
	Vector<int> vec1 = { 1, 2, 3 };
	Vector<int> vec2 = vec1;
	Vector<int> vec3 = { 4, 5, 6 };

	// WHEN
	vec3 = std::move(vec1);

	// THEN
	EXPECT_EQ(3, vec2.size());
	EXPECT_EQ(vec3, vec2);
	EXPECT_TRUE(vec1.empty());
}

TEST(VectorTest, PushBack) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };

	// WHEN
	vec.push_back(4);

	// THEN
	EXPECT_EQ(4, vec.size());
	EXPECT_EQ(6, vec.capacity());
	EXPECT_EQ(4, vec[3]);
}

TEST(VectorTest, EmplaceBack) {
	// GIVEN
	struct TestStruct {
		int m_a;
		int m_b;
		TestStruct() : TestStruct{ 0, 0 } {}
		TestStruct(int a, int b) : m_a{ a }, m_b{ b } {}
	};
	Vector<TestStruct> vec;

	// WHEN
	vec.emplace_back(5, 10);

	// THEN
	EXPECT_EQ(1, vec.size());
	EXPECT_EQ(1, vec.capacity());
	EXPECT_EQ(5, vec[0].m_a);
	EXPECT_EQ(10, vec[0].m_b);
}

TEST(VectorTest, PopBack) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };

	// WHEN
	vec.pop_back();

	// THEN
	EXPECT_EQ(2, vec.size());
	EXPECT_EQ(3, vec.capacity());
	EXPECT_THROW(vec.at(2), std::out_of_range);
}

TEST(VectorTest, EraseFirstIndex) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };
	const Vector<int> expected = { 2, 3 };

	// WHEN
	auto it = vec.erase(vec.begin());

	// THEN
	EXPECT_EQ(2, vec.size());
	EXPECT_EQ(3, vec.capacity());
	EXPECT_EQ(2, vec[0]);
	EXPECT_EQ(it, vec.begin());
	EXPECT_EQ(expected, vec);
	EXPECT_THROW(vec.at(2), std::out_of_range);
}

TEST(VectorTest, EraseMidIndex) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };
	const Vector<int> expected = { 1, 3 };

	// WHEN
	auto it = vec.erase(vec.begin() + 1);

	// THEN
	EXPECT_EQ(2, vec.size());
	EXPECT_EQ(3, vec.capacity());
	EXPECT_EQ(3, vec[1]);
	EXPECT_EQ(it, vec.begin() + 1);
	EXPECT_EQ(expected, vec);
	EXPECT_THROW(vec.at(2), std::out_of_range);
}

TEST(VectorTest, EraseLastIndex) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };
	const Vector<int> expected = { 1, 2 };

	// WHEN
	auto it = vec.erase(vec.end() - 1);

	// THEN
	EXPECT_EQ(2, vec.size());
	EXPECT_EQ(3, vec.capacity());
	EXPECT_EQ(2, vec[1]);
	EXPECT_EQ(it, vec.end());
	EXPECT_EQ(expected, vec);
	EXPECT_THROW(vec.at(2), std::out_of_range);
}

TEST(VectorTest, InsertFirstIndex) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };
	const Vector<int> expected = { 10, 1, 2, 3 };

	// WHEN
	auto it = vec.insert(vec.begin(), 10);

	// THEN
	EXPECT_EQ(4, vec.size());
	EXPECT_EQ(6, vec.capacity());
	EXPECT_EQ(10, vec[0]);
	EXPECT_EQ(it, vec.begin());
	EXPECT_EQ(3, vec.at(3));
	EXPECT_EQ(expected, vec);
}

TEST(VectorTest, InsertMidIndex) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };
	const Vector<int> expected = { 1, 10, 2, 3 };

	// WHEN
	auto it = vec.insert(vec.begin() + 1, 10);

	// THEN
	EXPECT_EQ(4, vec.size());
	EXPECT_EQ(6, vec.capacity());
	EXPECT_EQ(10, vec[1]);
	EXPECT_EQ(it, vec.begin() + 1);
	EXPECT_EQ(3, vec.at(3));
	EXPECT_EQ(expected, vec);
}

TEST(VectorTest, InsertLastIndex) {
	// GIVEN
	Vector<int> vec = { 1, 2, 3 };
	const Vector<int> expected = { 1, 2, 3, 10 };

	// WHEN
	auto it = vec.insert(vec.end(), 10);

	// THEN
	EXPECT_EQ(4, vec.size());
	EXPECT_EQ(6, vec.capacity());
	EXPECT_EQ(10, vec[3]);
	EXPECT_EQ(it, vec.end() - 1);
	EXPECT_EQ(expected, vec);
}