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
		std::priority_queue<Order> m_buyOrdersMaxHeap;
		std::priority_queue<Order, std::vector<Order>, std::greater<Order>> m_sellOrdersMinHeap;

		template <class T>
		std::vector<Order> getMatchedOrders(Order& order, T& matchingHeap, QuantityPriceMap& quantityPriceMap, const bool isBuy);
	public:
		OrderBookHeapImpl();

		std::vector<Order> addBuyOrder(Order&& order) override;
		std::vector<Order> addSellOrder(Order&& order) override;

		std::optional<Order> getBestBidOrder() const override;
		std::optional<Order> getBestAskOrder() const override;
	};

	template <class T>
	std::vector<Order> OrderBookHeapImpl::getMatchedOrders(Order& order, T& matchingHeap, QuantityPriceMap& quantityMap, const bool isBuy) {
		std::vector<Order> matchedOrders;
		while (!matchingHeap.empty()
			&& (isBuy && matchingHeap.top().price <= order.price
				|| !isBuy && matchingHeap.top().price >= order.price)) {
			Order matchedOrder = matchingHeap.top();
			matchingHeap.pop();
			// partial fill
			if (order.quantity < matchedOrder.quantity) {
				matchedOrder.quantity -= order.quantity;
				matchingHeap.push(matchedOrder);
				matchedOrder.quantity = order.quantity;
				matchedOrders.push_back(matchedOrder);
				quantityMap[matchedOrder.price] -= order.quantity;
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedOrder.quantity;
				quantityMap[matchedOrder.price] -= matchedOrder.quantity;
				matchedOrders.push_back(std::move(matchedOrder));
			}
		}
		return matchedOrders;
	}
}


