#include "pch.h"

#include "../implementations/orderbook_heapimpl.cpp"
#include "../implementations/orderbook_linkedlistmapimpl.cpp"
#include "../implementations/orderbook.cpp"

using namespace implementations;

class OrderBookTest :public ::testing::TestWithParam<std::shared_ptr<OrderBook>> {
protected:
	std::shared_ptr<OrderBook> orderbook;
};

INSTANTIATE_TEST_CASE_P(
	OrderBookParameterizedTest,
	OrderBookTest,
	::testing::Values(
		std::make_shared<OrderBookHeapImpl>(),
		std::make_shared<OrderBookLinkedListMapImpl>()
	));

TEST_P(OrderBookTest, addBuyOrderNoMatch) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 101, 200, 2 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook->addBuyOrder({ 99, 100, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	EXPECT_TRUE(matchedBuy.empty());
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(100, bestSell.value().price);
	EXPECT_EQ(200, bestSell.value().quantity);
	ASSERT_TRUE(bestBuy.has_value());
	EXPECT_EQ(99, bestBuy.value().price);
	EXPECT_EQ(100, bestBuy.value().quantity);
	EXPECT_EQ(400, orderbook->getQuantityAtAskPrice(102));
}

TEST_P(OrderBookTest, addBuyOrderFillMultiplePrices) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 101, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook->addBuyOrder({ 101, 100, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(1, matchedBuy.size());
	EXPECT_EQ(100, matchedBuy[0].price);
	EXPECT_EQ(100, matchedBuy[0].quantity);
	EXPECT_EQ(1, matchedBuy[0].timestamp);
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(100, bestSell.value().price);
	EXPECT_EQ(100, bestSell.value().quantity);
	EXPECT_FALSE(bestBuy.has_value());
}

TEST_P(OrderBookTest, addBuyOrderFillUseEarliestTimestamp) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 100, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 100, 400, 3 }).empty());
	EXPECT_EQ(900, orderbook->getQuantityAtAskPrice(100));
	std::vector<Order> matchedBuy = orderbook->addBuyOrder({ 101, 100, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(1, matchedBuy.size());
	EXPECT_EQ(100, matchedBuy[0].price);
	EXPECT_EQ(100, matchedBuy[0].quantity);
	EXPECT_EQ(1, matchedBuy[0].timestamp);
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(100, bestSell.value().price);
	EXPECT_EQ(100, bestSell.value().quantity);
	EXPECT_FALSE(bestBuy.has_value());
	EXPECT_EQ(800, orderbook->getQuantityAtAskPrice(100));
}

TEST_P(OrderBookTest, addBuyOrderPartialFill) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 101, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook->addBuyOrder({ 101, 700, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(2, matchedBuy.size());
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

TEST_P(OrderBookTest, addBuyOrderFillAllSells) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addSellOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 101, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addSellOrder({ 102, 400, 3 }).empty());
	std::vector<Order> matchedBuy = orderbook->addBuyOrder({ 102, 1000, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(3, matchedBuy.size());
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

TEST_P(OrderBookTest, addSellOrderNoMatch) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addBuyOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 99, 200, 2 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 98, 400, 3 }).empty());
	std::vector<Order> matchedSell = orderbook->addSellOrder({ 101, 100, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	EXPECT_TRUE(matchedSell.empty());
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(101, bestSell.value().price);
	EXPECT_EQ(100, bestSell.value().quantity);
	ASSERT_TRUE(bestBuy.has_value());
	EXPECT_EQ(100, bestBuy.value().price);
	EXPECT_EQ(200, bestBuy.value().quantity);
	EXPECT_EQ(400, orderbook->getQuantityAtBidPrice(98));
}

TEST_P(OrderBookTest, addSellOrderFillMultiplePrices) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addBuyOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 99, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 98, 400, 3 }).empty());
	std::vector<Order> matchedSell = orderbook->addSellOrder({ 100, 100, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(1, matchedSell.size());
	EXPECT_EQ(100, matchedSell[0].price);
	EXPECT_EQ(100, matchedSell[0].quantity);
	EXPECT_EQ(1, matchedSell[0].timestamp);
	ASSERT_TRUE(bestBuy.has_value());
	EXPECT_EQ(100, bestBuy.value().price);
	EXPECT_EQ(100, bestBuy.value().quantity);
	EXPECT_FALSE(bestSell.has_value());
}

TEST_P(OrderBookTest, addSellOrderFillUseEarliestTimestamp) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addBuyOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 100, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 100, 400, 3 }).empty());
	EXPECT_EQ(900, orderbook->getQuantityAtBidPrice(100));
	std::vector<Order> matchedSell = orderbook->addSellOrder({ 99, 100, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(1, matchedSell.size());
	EXPECT_EQ(100, matchedSell[0].price);
	EXPECT_EQ(100, matchedSell[0].quantity);
	EXPECT_EQ(1, matchedSell[0].timestamp);
	ASSERT_TRUE(bestBuy.has_value());
	EXPECT_EQ(100, bestBuy.value().price);
	EXPECT_EQ(100, bestBuy.value().quantity);
	EXPECT_FALSE(bestSell.has_value());
	EXPECT_EQ(800, orderbook->getQuantityAtBidPrice(100));
}

TEST_P(OrderBookTest, addSellOrderPartialFill) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addBuyOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 99, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 98, 400, 3 }).empty());
	std::vector<Order> matchedSell = orderbook->addSellOrder({ 99, 700, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(2, matchedSell.size());
	EXPECT_EQ(100, matchedSell[0].price);
	EXPECT_EQ(200, matchedSell[0].quantity);
	EXPECT_EQ(1, matchedSell[0].timestamp);
	EXPECT_EQ(99, matchedSell[1].price);
	EXPECT_EQ(300, matchedSell[1].quantity);
	EXPECT_EQ(2, matchedSell[1].timestamp);
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(99, bestSell.value().price);
	EXPECT_EQ(200, bestSell.value().quantity);
	ASSERT_TRUE(bestBuy.has_value());
	EXPECT_EQ(98, bestBuy.value().price);
	EXPECT_EQ(400, bestBuy.value().quantity);
}

TEST_P(OrderBookTest, addSellOrderFillAllSells) {
	// GIVEN
	std::shared_ptr<OrderBook> orderbook = GetParam();

	// WHEN
	EXPECT_TRUE(orderbook->addBuyOrder({ 100, 200, 1 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 99, 300, 2 }).empty());
	EXPECT_TRUE(orderbook->addBuyOrder({ 98, 400, 3 }).empty());
	std::vector<Order> matchedSell = orderbook->addSellOrder({ 98, 1000, 4 });
	std::optional<Order> bestSell = orderbook->getBestAskOrder();
	std::optional<Order> bestBuy = orderbook->getBestBidOrder();

	// THEN
	ASSERT_EQ(3, matchedSell.size());
	EXPECT_EQ(100, matchedSell[0].price);
	EXPECT_EQ(200, matchedSell[0].quantity);
	EXPECT_EQ(1, matchedSell[0].timestamp);
	EXPECT_EQ(99, matchedSell[1].price);
	EXPECT_EQ(300, matchedSell[1].quantity);
	EXPECT_EQ(2, matchedSell[1].timestamp);
	EXPECT_EQ(98, matchedSell[2].price);
	EXPECT_EQ(400, matchedSell[2].quantity);
	EXPECT_EQ(3, matchedSell[2].timestamp);
	EXPECT_FALSE(bestBuy.has_value());
	ASSERT_TRUE(bestSell.has_value());
	EXPECT_EQ(98, bestSell.value().price);
	EXPECT_EQ(100, bestSell.value().quantity);
}