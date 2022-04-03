#include "orderbook_heapimpl.h"

namespace implementations {
	OrderBookHeapImpl::OrderBookHeapImpl()
		: OrderBook()
		, m_buyOrdersMaxHeap()
		, m_sellOrdersMinHeap() {}


	std::vector<Order> OrderBookHeapImpl::addBuyOrder(Order&& order) {
		const std::vector<Order> matchedOrders = getMatchedOrders(order, m_sellOrdersMinHeap, m_quantityAtAskPrice, true);
		if (order.quantity > 0) {
			m_quantityAtBidPrice[order.price] += order.quantity;
			m_buyOrdersMaxHeap.push(std::move(order));
		}
		return matchedOrders;
	}

	std::vector<Order> OrderBookHeapImpl::addSellOrder(Order&& order) {
		const std::vector<Order> matchedOrders = getMatchedOrders(order, m_buyOrdersMaxHeap, m_quantityAtBidPrice, false);
		if (order.quantity > 0) {
			m_quantityAtAskPrice[order.price] += order.quantity;
			m_sellOrdersMinHeap.push(std::move(order));
		}
		return matchedOrders;
	}

	std::optional<Order> OrderBookHeapImpl::getBestBidOrder() const {
		if (m_buyOrdersMaxHeap.empty()) {
			return {};
		}
		return m_buyOrdersMaxHeap.top();
	}

	std::optional<Order> OrderBookHeapImpl::getBestAskOrder() const {
		if (m_sellOrdersMinHeap.empty()) {
			return {};
		}
		return m_sellOrdersMinHeap.top();
	}
}