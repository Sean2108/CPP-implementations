#pragma once

#include "orderbook.h"

#include <map>
#include <memory>

namespace implementations {
	/*
	* insertion time complexity
	*    - worst case O(nlogn) (eg 1 buy order fills all sell orders)
	*    - however, the average case should be closer to O(logn) as orders can only be removed at most once
	* query quantity for price time complexity - O(n) TODO: this can be improved to O(1) by storing the quantities at each price
	*/
	class OrderBookLinkedListMapImpl : public OrderBook {
		struct OrderNode : public Order {
			std::shared_ptr<OrderNode> next;

			OrderNode(Order&& order);
		};
		using OrderNodePtr = std::shared_ptr<OrderNode>;
		using LinkedListMap = std::map<size_t, OrderNodePtr>;
		LinkedListMap m_buyOrders, m_sellOrders;
	public:
		OrderBookLinkedListMapImpl();

		std::vector<Order> addBuyOrder(Order order) override;
		std::vector<Order> addSellOrder(Order order) override;

		std::optional<Order> getBestBidOrder() const override;
		std::optional<Order> getBestAskOrder() const override;

		size_t getQuantityAtBidPrice(const size_t price) const override;
		size_t getQuantityAtAskPrice(const size_t price) const override;
	};
}

