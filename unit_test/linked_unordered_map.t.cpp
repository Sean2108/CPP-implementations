#include "pch.h"

#include "../implementations/linked_unordered_map.cpp"
#include <string>

using namespace implementations;

TEST(LinkedUnorderedMapTest, InsertAtHead) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN
	EXPECT_TRUE(map.insertAtHead(1, "a"));
	EXPECT_TRUE(map.insertAtHead(2, "b"));
	EXPECT_TRUE(map.insertAtHead(3, "c"));

	// THEN
	ASSERT_EQ(3, map.size());
	EXPECT_EQ("a", map[1]);
	EXPECT_EQ("b", map[2]);
	EXPECT_EQ("c", map[3]);
	map[3] = "d";
	EXPECT_EQ("d", map[3]);
}

TEST(LinkedUnorderedMapTest, InsertAtHeadFalseForDuplicate) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN
	EXPECT_TRUE(map.insertAtHead(1, "a"));

	// THEN
	EXPECT_FALSE(map.insertAtHead(1, "b"));
}

TEST(LinkedUnorderedMapTest, InsertAtTail) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN
	EXPECT_TRUE(map.insertAtTail(1, "a"));
	EXPECT_TRUE(map.insertAtTail(2, "b"));
	EXPECT_TRUE(map.insertAtTail(3, "c"));

	// THEN
	ASSERT_EQ(3, map.size());
	EXPECT_EQ("a", map[1]);
	EXPECT_EQ("b", map[2]);
	EXPECT_EQ("c", map[3]);
}

TEST(LinkedUnorderedMapTest, InsertAtTailFalseForDuplicate) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN
	EXPECT_TRUE(map.insertAtTail(1, "a"));

	// THEN
	EXPECT_FALSE(map.insertAtTail(1, "b"));
}

TEST(LinkedUnorderedMapTest, Remove) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN
	EXPECT_TRUE(map.insertAtTail(1, "a"));
	ASSERT_EQ(1, map.size());
	map.remove(1);

	// THEN
	EXPECT_EQ(0, map.size());
	EXPECT_TRUE(map.empty());
}

TEST(LinkedUnorderedMapTest, AcessingMissingKeyThrows) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN

	// THEN
	EXPECT_THROW(map[1], std::invalid_argument);
}

TEST(LinkedUnorderedMapTest, MoveToEnd) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN
	EXPECT_TRUE(map.insertAtTail(1, "a"));
	EXPECT_TRUE(map.insertAtHead(2, "b"));
	EXPECT_TRUE(map.insertAtTail(3, "c"));
	map.moveToEnd(2);

	// THEN
	ASSERT_EQ(3, map.size());
	EXPECT_EQ(2, map.remove(false).first);
	EXPECT_EQ(1, map.remove(true).first);
	EXPECT_EQ(3, map.remove(false).first);
	EXPECT_TRUE(map.empty());
}

TEST(LinkedUnorderedMapTest, MoveToEndThrowsForMissingKey) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN
	EXPECT_TRUE(map.insertAtTail(1, "a"));

	// THEN
	EXPECT_THROW(map.moveToEnd(2), std::invalid_argument);
}

TEST(LinkedUnorderedMapTest, RemoveThrowsForEmptyMap) {
	// GIVEN
	LinkedUnorderedMap<int, std::string> map;

	// WHEN

	// THEN
	EXPECT_THROW(map.remove(true), std::runtime_error);
}