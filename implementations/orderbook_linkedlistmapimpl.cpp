#include "orderbook_linkedlistmapimpl.h"

namespace implementations {
	OrderBookLinkedListMapImpl::OrderNode::OrderNode(Order&& order)
		: Order(std::move(order))
		, next(nullptr) {}

	OrderBookLinkedListMapImpl::OrderBookLinkedListMapImpl()
		: OrderBook()
		, m_buyOrders()
		, m_sellOrders() {}

	std::vector<Order> OrderBookLinkedListMapImpl::getMatchedOrders(Order& order, LinkedListMap& matchingOrders, QuantityPriceMap& matchingQuantityMap, const bool isBuy) {
		std::vector<Order> matchedOrders;
		while (!matchingOrders.empty() &&
			// compare against largest buy or smallest sell, depending on the side
			(isBuy && matchingOrders.begin()->second->price <= order.price
				|| !isBuy && matchingOrders.rbegin()->second->price >= order.price)) {
			OrderNodePtr matchedOrderPtr = isBuy ? matchingOrders.begin()->second : matchingOrders.rbegin()->second;
			// partial fill
			if (order.quantity < matchedOrderPtr->quantity) {
				const size_t newQuantity = matchedOrderPtr->quantity - order.quantity;
				matchedOrderPtr->quantity = order.quantity;
				matchedOrders.push_back(*matchedOrderPtr);
				matchedOrderPtr->quantity = newQuantity;
				matchingQuantityMap[matchedOrderPtr->price] -= order.quantity;
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedOrderPtr->quantity;
				matchingQuantityMap[matchedOrderPtr->price] -= matchedOrderPtr->quantity;
				if (!matchedOrderPtr->next) {
					matchingOrders.erase(isBuy ? matchingOrders.begin() : std::next(m_buyOrders.rbegin()).base());
				}
				else {
					matchingOrders.begin()->second = matchedOrderPtr->next;
				}
				matchedOrders.push_back(std::move(*matchedOrderPtr));
			}
		}
		return matchedOrders;
	}

	void OrderBookLinkedListMapImpl::addOrderNode(Order&& order, LinkedListMap& sideMap) {
		const size_t price = order.price;
		auto it = sideMap.find(price);
		const OrderNodePtr newOrderNode = std::make_shared<OrderNode>(std::move(order));
		if (it != sideMap.cend()) {
			OrderNodePtr currentNode = it->second;
			while (currentNode && currentNode->next) {
				currentNode = currentNode->next;
			}
			currentNode->next = std::move(newOrderNode);
		}
		else {
			sideMap.emplace(price, std::move(newOrderNode));
		}
	}

	std::vector<Order> OrderBookLinkedListMapImpl::addBuyOrder(Order&& order) {
		const std::vector<Order> matchedOrders = getMatchedOrders(order, m_sellOrders, m_quantityAtAskPrice, true);
		if (order.quantity > 0) {
			m_quantityAtBidPrice[order.price] += order.quantity;
			addOrderNode(std::move(order), m_buyOrders);
		}
		return matchedOrders;
	}

	std::vector<Order> OrderBookLinkedListMapImpl::addSellOrder(Order&& order) {
		const std::vector<Order> matchedOrders = getMatchedOrders(order, m_buyOrders, m_quantityAtBidPrice, false);
		if (order.quantity > 0) {
			m_quantityAtAskPrice[order.price] += order.quantity;
			addOrderNode(std::move(order), m_sellOrders);
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
}