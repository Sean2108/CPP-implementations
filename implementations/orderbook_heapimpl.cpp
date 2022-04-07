#include "orderbook_heapimpl.h"

namespace implementations {

	bool OrderBookHeapImpl::minHeapComparator(const Order& a, const Order& b) {
		return std::tie(a.price, a.timestamp, a.quantity) > std::tie(b.price, b.timestamp, b.quantity);
	}

	bool OrderBookHeapImpl::maxHeapComparator(const Order& a, const Order& b) {
		return std::forward_as_tuple(a.price, -int(a.timestamp), a.quantity) < std::forward_as_tuple(b.price, -int(b.timestamp), b.quantity);
	}

	OrderBookHeapImpl::OrderBookHeapImpl()
		: OrderBook()
		, m_buyOrdersMaxHeap(maxHeapComparator)
		, m_sellOrdersMinHeap(minHeapComparator) {}

	std::vector<Order> OrderBookHeapImpl::getMatchedOrders(Order& order, PriorityQueue& matchingHeap, QuantityPriceMap& quantityMap, const bool isBuy) {
		std::vector<Order> matchedOrders;
		std::lock_guard<std::mutex> lock(isBuy ? m_sellOrderMutex : m_buyOrderMutex);
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

	std::vector<Order> OrderBookHeapImpl::addBuyOrder(Order&& order) {
		const std::vector<Order> matchedOrders = getMatchedOrders(order, m_sellOrdersMinHeap, m_quantityAtAskPrice, true);
		if (order.quantity > 0) {
			std::lock_guard<std::mutex> lock(m_buyOrderMutex);
			m_quantityAtBidPrice[order.price] += order.quantity;
			m_buyOrdersMaxHeap.push(std::move(order));
		}
		return matchedOrders;
	}

	std::vector<Order> OrderBookHeapImpl::addSellOrder(Order&& order) {
		const std::vector<Order> matchedOrders = getMatchedOrders(order, m_buyOrdersMaxHeap, m_quantityAtBidPrice, false);
		if (order.quantity > 0) {
			std::lock_guard<std::mutex> lock(m_sellOrderMutex);
			m_quantityAtAskPrice[order.price] += order.quantity;
			m_sellOrdersMinHeap.push(std::move(order));
		}
		return matchedOrders;
	}

	std::optional<Order> OrderBookHeapImpl::getBestBidOrder() const {
		std::lock_guard<std::mutex> lock(m_buyOrderMutex);
		if (m_buyOrdersMaxHeap.empty()) {
			return {};
		}
		return m_buyOrdersMaxHeap.top();
	}

	std::optional<Order> OrderBookHeapImpl::getBestAskOrder() const {
		std::lock_guard<std::mutex> lock(m_sellOrderMutex);
		if (m_sellOrdersMinHeap.empty()) {
			return {};
		}
		return m_sellOrdersMinHeap.top();
	}
}