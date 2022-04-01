#pragma once

#include <vector>
#include <optional>

namespace implementations {
	struct Order {
		size_t price;
		size_t quantity;
		size_t timestamp;

		Order(size_t price, size_t quantity, size_t timestamp);
		friend bool operator<(const Order& order1, const Order& order2);
	};
	bool operator>(const Order& order1, const Order& order2);
	bool operator<=(const Order& order1, const Order& order2);
	bool operator>=(const Order& order1, const Order& order2);

	class OrderBook {
	public:
		virtual std::vector<Order> addBuyOrder(Order order) = 0;
		virtual std::vector<Order> addSellOrder(Order order) = 0;

		virtual std::optional<Order> getBestBidOrder() const = 0;
		virtual std::optional<Order> getBestAskOrder() const = 0;

		virtual size_t getQuantityAtBidPrice(const size_t price) const = 0;
		virtual size_t getQuantityAtAskPrice(const size_t price) const = 0;
	};
}

