#pragma once

#include "orderbook.h"

#include <optional>
#include <queue>
#include <vector>

namespace implementations {
	/*
	* insertion time complexity
	*    - worst case O(nlogn) (eg 1 buy order fills all sell orders)
	*    - however, the average case should be closer to O(logn) as orders can only be removed at most once
	* query quantity for price time complexity - O(1)
	*/
	class OrderBookHeapImpl : public OrderBook
	{
		static bool minHeapComparator(const Order& a, const Order& b);
		static bool maxHeapComparator(const Order& a, const Order& b);

		using PriorityQueue = std::priority_queue<Order, std::vector<Order>, decltype(&OrderBookHeapImpl::maxHeapComparator)>;

		PriorityQueue m_buyOrdersMaxHeap;
		PriorityQueue m_sellOrdersMinHeap;

		std::vector<Order> getMatchedOrders(Order& order, PriorityQueue& matchingHeap, QuantityPriceMap& quantityPriceMap, const bool isBuy);
	public:
		OrderBookHeapImpl();

		std::vector<Order> addBuyOrder(Order&& order) override;
		std::vector<Order> addSellOrder(Order&& order) override;

		std::optional<Order> getBestBidOrder() const override;
		std::optional<Order> getBestAskOrder() const override;
	};
}


