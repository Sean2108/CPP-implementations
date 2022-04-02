#pragma once

#include "orderbook.h"

#include <optional>
#include <queue>
#include <unordered_map>
#include <vector>

namespace implementations {
	class OrderBookHeapImpl : public OrderBook
	{
		std::priority_queue<Order> buyOrdersMaxHeap;
		std::priority_queue<Order, std::vector<Order>, std::greater<Order>> sellOrdersMinHeap;
		std::unordered_map<size_t, size_t> quantityAtBidPrice;
		std::unordered_map<size_t, size_t> quantityAtAskPrice;
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


