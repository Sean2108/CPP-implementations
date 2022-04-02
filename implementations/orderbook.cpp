#include "orderbook.h"

#include <tuple>

namespace implementations {
	Order::Order(size_t price, size_t quantity, size_t timestamp)
		: price(price)
		, quantity(quantity)
		, timestamp(timestamp) {}

	bool operator<(const Order& order1, const Order& order2) {
		return std::forward_as_tuple(order1.price, -int(order1.timestamp), order1.quantity) <
			std::forward_as_tuple(order2.price, -int(order2.timestamp), order2.quantity);
	}

	bool operator>(const Order& order1, const Order& order2) {
		return std::tie(order1.price, order1.timestamp, order1.quantity) >
			std::tie(order2.price, order2.timestamp, order2.quantity);
	}

	inline bool operator<=(const Order& order1, const Order& order2) {
		return !(order1 > order2);
	}

	inline bool operator>=(const Order& order1, const Order& order2) {
		return !(order1 < order2);
	}
}