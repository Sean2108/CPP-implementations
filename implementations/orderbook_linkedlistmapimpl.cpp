#include "orderbook_linkedlistmapimpl.h"

namespace implementations {
	OrderBookLinkedListMapImpl::OrderNode::OrderNode(Order&& order)
		: Order(std::move(order))
		, next(nullptr) {}

	OrderBookLinkedListMapImpl::OrderBookLinkedListMapImpl()
		: m_buyOrders()
		, m_sellOrders() {}

	std::vector<Order> OrderBookLinkedListMapImpl::addBuyOrder(Order order) {
		std::vector<Order> matchedOrders;
		while (!m_sellOrders.empty() && 
			m_sellOrders.begin()->second->price <= order.price) {
			OrderNodePtr matchedSellPtr = m_sellOrders.begin()->second;
			// partial fill
			if (order.quantity < matchedSellPtr->quantity) {
				matchedSellPtr->quantity -= order.quantity;
				matchedOrders.emplace_back(matchedSellPtr->price, order.quantity, matchedSellPtr->timestamp);
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedSellPtr->quantity;
				if (!matchedSellPtr->next) {
					m_sellOrders.erase(m_sellOrders.begin());
				}
				else {
					m_sellOrders.begin()->second = matchedSellPtr->next;
				}
				matchedOrders.push_back(std::move(*matchedSellPtr));
			}
		}
		if (order.quantity > 0) {
			const size_t price = order.price;
			auto it = m_buyOrders.find(price);
			const OrderNodePtr newOrderNode = std::make_shared<OrderNode>(std::move(order));
			if (it != m_buyOrders.cend()) {
				OrderNodePtr currentNode = it->second;
				while (currentNode && currentNode->next) {
					currentNode = currentNode->next;
				}
				currentNode->next = newOrderNode;
			}
			else {
				m_buyOrders.emplace(order.price, newOrderNode);
			}
		}
		return matchedOrders;
	}

	std::vector<Order> OrderBookLinkedListMapImpl::addSellOrder(Order order) {
		std::vector<Order> matchedOrders;
		while (!m_buyOrders.empty() &&
			m_buyOrders.rbegin()->second->price >= order.price) {
			OrderNodePtr matchedBuyPtr = m_buyOrders.rbegin()->second;
			// partial fill
			if (order.quantity < matchedBuyPtr->quantity) {
				matchedBuyPtr->quantity -= order.quantity;
				matchedOrders.emplace_back(matchedBuyPtr->price, order.quantity, matchedBuyPtr->timestamp);
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedBuyPtr->quantity;
				if (!matchedBuyPtr->next) {
					m_buyOrders.erase(std::next(m_buyOrders.rbegin()).base());
				}
				else {
					m_buyOrders.rbegin()->second = matchedBuyPtr->next;
				}
				matchedOrders.push_back(std::move(*matchedBuyPtr));
			}
		}
		if (order.quantity > 0) {
			const size_t price = order.price;
			auto it = m_sellOrders.find(price);
			const OrderNodePtr newOrderNode = std::make_shared<OrderNode>(std::move(order));
 			if (it != m_sellOrders.cend()) {
				OrderNodePtr currentNode = it->second;
				while (currentNode && currentNode->next) {
					currentNode = currentNode->next;
				}
				currentNode->next = newOrderNode;
			}
			else {
				m_sellOrders.emplace(order.price, newOrderNode);
			}
		}
		return matchedOrders;
	}

	std::optional<Order> OrderBookLinkedListMapImpl::getBestBidOrder() const {
		if (m_buyOrders.empty()) {
			return {};
		}
		return *(m_buyOrders.rbegin()->second);
	}

	std::optional<Order> OrderBookLinkedListMapImpl::getBestAskOrder() const {
		if (m_sellOrders.empty()) {
			return {};
		}
		return *(m_sellOrders.begin()->second);
	}

	size_t OrderBookLinkedListMapImpl::getQuantityAtBidPrice(const size_t price) const {
		const auto it = m_buyOrders.find(price);
		size_t totalQuantity = 0;
		if (it != m_buyOrders.cend()) {
			OrderNodePtr currentNode = it->second;
			while (currentNode) {
				totalQuantity += currentNode->quantity;
				currentNode = currentNode->next;
			}
		}
		return totalQuantity;
	}

	size_t OrderBookLinkedListMapImpl::getQuantityAtAskPrice(const size_t price) const {
		const auto it = m_sellOrders.find(price);
		size_t totalQuantity = 0;
		if (it != m_sellOrders.cend()) {
			OrderNodePtr currentNode = it->second;
			while (currentNode) {
				totalQuantity += currentNode->quantity;
				currentNode = currentNode->next;
			}
		}
		return totalQuantity;
	}
}