#include "orderbook_heapimpl.h"

namespace implementations {
	OrderBookHeapImpl::OrderBookHeapImpl()
		: m_buyOrdersMaxHeap()
		, m_sellOrdersMinHeap()
		, m_quantityAtBidPrice()
		, m_quantityAtAskPrice() {}

	std::vector<Order> OrderBookHeapImpl::addBuyOrder(Order order) {
		std::vector<Order> matchedOrders;
		while (!m_sellOrdersMinHeap.empty()
			&& m_sellOrdersMinHeap.top().price <= order.price
			&& order.quantity > 0) {
			Order matchedSell = m_sellOrdersMinHeap.top();
			m_sellOrdersMinHeap.pop();
			// partial fill
			if (order.quantity < matchedSell.quantity) {
				matchedSell.quantity -= order.quantity;
				m_sellOrdersMinHeap.push(matchedSell);
				matchedOrders.emplace_back(matchedSell.price, order.quantity, matchedSell.timestamp);
				m_quantityAtAskPrice[matchedSell.price] -= order.quantity;
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedSell.quantity;
				m_quantityAtAskPrice[matchedSell.price] -= matchedSell.quantity;
				matchedOrders.push_back(std::move(matchedSell));
			}
		}
		if (order.quantity > 0) {
			m_quantityAtBidPrice[order.price] += order.quantity;
			m_buyOrdersMaxHeap.push(std::move(order));
		}
		return matchedOrders;
	}

	std::vector<Order> OrderBookHeapImpl::addSellOrder(Order order) {
		std::vector<Order> matchedOrders;
		while (!m_buyOrdersMaxHeap.empty()
			&& m_buyOrdersMaxHeap.top().price >= order.price
			&& order.quantity > 0) {
			Order matchedBuy = m_buyOrdersMaxHeap.top();
			m_buyOrdersMaxHeap.pop();
			// partial fill
			if (order.quantity < matchedBuy.quantity) {
				matchedBuy.quantity -= order.quantity;
				m_buyOrdersMaxHeap.push(matchedBuy);
				matchedOrders.emplace_back(matchedBuy.price, order.quantity, matchedBuy.timestamp);
				m_quantityAtBidPrice[matchedBuy.price] -= order.quantity;
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedBuy.quantity;
				m_quantityAtBidPrice[matchedBuy.price] -= matchedBuy.quantity;
				matchedOrders.push_back(std::move(matchedBuy));
			}
		}
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

	size_t OrderBookHeapImpl::getQuantityAtBidPrice(const size_t price) const {
		const auto it = m_quantityAtBidPrice.find(price);
		if (it != m_quantityAtBidPrice.cend()) {
			return it->second;
		}
		return 0;
	}

	size_t OrderBookHeapImpl::getQuantityAtAskPrice(const size_t price) const {
		const auto it = m_quantityAtAskPrice.find(price);
		if (it != m_quantityAtAskPrice.cend()) {
			return it->second;
		}
		return 0;
	}
}