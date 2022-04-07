#pragma once

#include <mutex>
#include <optional>
#include <vector>
#include <unordered_map>

namespace implementations {
	struct Order {
		size_t price;
		size_t quantity;
		size_t timestamp;

		Order(size_t price, size_t quantity, size_t timestamp);
		friend bool operator<(const Order& order1, const Order& order2);
		friend bool operator>(const Order& order1, const Order& order2);
	};
	bool operator<=(const Order& order1, const Order& order2);
	bool operator>=(const Order& order1, const Order& order2);

	using QuantityPriceMap = std::unordered_map<size_t, size_t>;

	class OrderBook {
	protected:
		QuantityPriceMap m_quantityAtBidPrice, m_quantityAtAskPrice;
		mutable std::mutex m_buyOrderMutex;
		mutable std::mutex m_sellOrderMutex;
	public:
		OrderBook();
		virtual std::vector<Order> addBuyOrder(Order&& order) = 0;
		virtual std::vector<Order> addSellOrder(Order&& order) = 0;

		virtual std::optional<Order> getBestBidOrder() const = 0;
		virtual std::optional<Order> getBestAskOrder() const = 0;

		size_t getQuantityAtBidPrice(const size_t price) const;
		size_t getQuantityAtAskPrice(const size_t price) const;
	};
}

