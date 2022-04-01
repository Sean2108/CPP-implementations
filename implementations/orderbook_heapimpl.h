#pragma once

#include "orderbook.h"

#include <optional>
#include <queue>
#include <vector>

namespace implementations {
	class OrderBookHeapImpl : public OrderBook
	{
		std::priority_queue<Order, std::vector<Order>> buyOrdersMaxHeap;
		std::priority_queue<Order, std::vector<Order>, std::greater<Order>> sellOrdersMinHeap;
	public:
		OrderBookHeapImpl();

		std::vector<Order> addBuyOrder(Order order) override;
		std::vector<Order> addSellOrder(Order order) override;

		std::optional<Order> getBestBidOrder() const override;
		std::optional<Order> getBestAskOrder() const override;

		size_t getQuantityAtBidPrice(const size_t price) const override;
		size_t getQuantityAtAskPrice(const size_t price) const override;
	};
}


