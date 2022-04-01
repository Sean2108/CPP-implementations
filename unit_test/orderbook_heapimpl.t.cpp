#include "pch.h"

#include "../implementations/orderbook_heapimpl.cpp"
#include "../implementations/orderbook.cpp"

using namespace implementations;

TEST(OrderBookHeapImplTest, addBuyOrderNoMatch) {
	// GIVEN
	OrderBookHeapImpl orderbook;

	// WHEN
	EXPECT_TRUE(orderbook.addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 101, 200, 2 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook.addBuyOrder({ 99, 100, 4 });

	// THEN
	EXPECT_TRUE(matchedBuy.empty());
}

TEST(OrderBookHeapImplTest, addBuyOrderFillMultiplePrices) {
	// GIVEN
	OrderBookHeapImpl orderbook;

	// WHEN
	EXPECT_TRUE(orderbook.addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 101, 300, 2 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook.addBuyOrder({ 101, 100, 4 });
	std::optional<Order> bestSell = orderbook.getBestAskOrder();
	std::optional<Order> bestBuy = orderbook.getBestBidOrder();

	// THEN
	ASSERT_TRUE(1, matchedBuy.size());
	EXPECT_EQ(100, matchedBuy[0].price);
	EXPECT_EQ(100, matchedBuy[0].quantity);
	EXPECT_EQ(1, matchedBuy[0].timestamp);
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(100, bestSell.value().price);
	EXPECT_EQ(100, bestSell.value().quantity);
	EXPECT_FALSE(bestBuy.has_value());
}

TEST(OrderBookHeapImplTest, addBuyOrderFillUseEarliestTimestamp) {
	// GIVEN
	OrderBookHeapImpl orderbook;

	// WHEN
	EXPECT_TRUE(orderbook.addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 100, 300, 2 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 100, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook.addBuyOrder({ 101, 100, 4 });
	std::optional<Order> bestSell = orderbook.getBestAskOrder();
	std::optional<Order> bestBuy = orderbook.getBestBidOrder();

	// THEN
	ASSERT_TRUE(1, matchedBuy.size());
	EXPECT_EQ(100, matchedBuy[0].price);
	EXPECT_EQ(100, matchedBuy[0].quantity);
	EXPECT_EQ(1, matchedBuy[0].timestamp);
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(100, bestSell.value().price);
	EXPECT_EQ(100, bestSell.value().quantity);
	EXPECT_FALSE(bestBuy.has_value());
}

TEST(OrderBookHeapImplTest, addBuyOrderPartialFill) {
	// GIVEN
	OrderBookHeapImpl orderbook;

	// WHEN
	EXPECT_TRUE(orderbook.addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 101, 300, 2 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook.addBuyOrder({ 101, 700, 4 });
	std::optional<Order> bestSell = orderbook.getBestAskOrder();
	std::optional<Order> bestBuy = orderbook.getBestBidOrder();

	// THEN
	ASSERT_TRUE(2, matchedBuy.size());
	EXPECT_EQ(100, matchedBuy[0].price);
	EXPECT_EQ(200, matchedBuy[0].quantity);
	EXPECT_EQ(1, matchedBuy[0].timestamp);
	EXPECT_EQ(101, matchedBuy[1].price);
	EXPECT_EQ(300, matchedBuy[1].quantity);
	EXPECT_EQ(2, matchedBuy[1].timestamp);
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(102, bestSell.value().price);
	EXPECT_EQ(400, bestSell.value().quantity);
	ASSERT_TRUE(bestBuy.has_value());
	EXPECT_EQ(101, bestBuy.value().price);
	EXPECT_EQ(200, bestBuy.value().quantity);
}

TEST(OrderBookHeapImplTest, addBuyOrderFillAllSells) {
	// GIVEN
	OrderBookHeapImpl orderbook;

	// WHEN
	EXPECT_TRUE(orderbook.addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 101, 300, 2 }).empty());
	EXPECT_TRUE(orderbook.addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook.addBuyOrder({ 102, 1000, 4 });
	std::optional<Order> bestSell = orderbook.getBestAskOrder();
	std::optional<Order> bestBuy = orderbook.getBestBidOrder();

	// THEN
	ASSERT_TRUE(3, matchedBuy.size());
	EXPECT_EQ(100, matchedBuy[0].price);
	EXPECT_EQ(200, matchedBuy[0].quantity);
	EXPECT_EQ(1, matchedBuy[0].timestamp);
	EXPECT_EQ(101, matchedBuy[1].price);
	EXPECT_EQ(300, matchedBuy[1].quantity);
	EXPECT_EQ(2, matchedBuy[1].timestamp);
	EXPECT_EQ(102, matchedBuy[2].price);
	EXPECT_EQ(400, matchedBuy[2].quantity);
	EXPECT_EQ(3, matchedBuy[2].timestamp);
	EXPECT_FALSE(bestSell.has_value());
	ASSERT_TRUE(bestBuy.has_value());
	EXPECT_EQ(102, bestBuy.value().price);
	EXPECT_EQ(100, bestBuy.value().quantity);
}