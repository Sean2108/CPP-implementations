#include "pch.h"

#include "../implementations/lru_cache.cpp"
#include "../implementations/linked_unordered_map.cpp"

using namespace implementations;

TEST(LruCacheTest, GetMovesItemToEnd) {
	// GIVEN
	LruCache<int, int> cache(2);

	// WHEN
	cache.push(1, 2);
	cache.push(2, 3);
	cache.get(1);
	cache.push(3, 4);

	// THEN
	EXPECT_EQ(2, cache.size());
	ASSERT_TRUE(cache.hasKey(1));
	EXPECT_EQ(2, cache[1]);
	EXPECT_TRUE(cache.hasKey(3));
	EXPECT_FALSE(cache.hasKey(2));
}

TEST(LruCacheTest, PushUpdateMovesItemToEnd) {
	// GIVEN
	LruCache<int, int> cache(2);

	// WHEN
	cache.push(1, 2);
	cache.push(2, 3);
	cache.push(1, 5);
	cache.push(3, 4);

	// THEN
	EXPECT_EQ(2, cache.size());
	ASSERT_TRUE(cache.hasKey(1));
	EXPECT_EQ(5, cache[1]);
	EXPECT_TRUE(cache.hasKey(3));
	EXPECT_FALSE(cache.hasKey(2));
}