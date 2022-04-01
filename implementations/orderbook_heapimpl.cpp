#include "orderbook_heapimpl.h"

namespace implementations {
	OrderBookHeapImpl::OrderBookHeapImpl()
		: buyOrdersMaxHeap()
		, sellOrdersMinHeap() {}

	std::vector<Order> OrderBookHeapImpl::addBuyOrder(Order order) {
		std::vector<Order> matchedOrders;
		while (!sellOrdersMinHeap.empty()
			&& sellOrdersMinHeap.top().price <= order.price
			&& order.quantity > 0) {
			Order matchedSell = sellOrdersMinHeap.top();
			sellOrdersMinHeap.pop();
			// partial fill
			if (order.quantity < matchedSell.quantity) {
				matchedSell.quantity -= order.quantity;
				sellOrdersMinHeap.push(matchedSell);
				matchedOrders.emplace_back(matchedSell.price, order.quantity, matchedSell.timestamp);
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedSell.quantity;
				matchedOrders.push_back(std::move(matchedSell));
			}
		}
		if (order.quantity > 0) {
			buyOrdersMaxHeap.push(std::move(order));
		}
		return matchedOrders;
	}

	std::vector<Order> OrderBookHeapImpl::addSellOrder(Order order) {
		std::vector<Order> matchedOrders;
		while (!buyOrdersMaxHeap.empty()
			&& buyOrdersMaxHeap.top().price >= order.price
			&& order.quantity > 0) {
			Order matchedBuy = buyOrdersMaxHeap.top();
			buyOrdersMaxHeap.pop();
			// partial fill
			if (order.quantity < matchedBuy.quantity) {
				matchedBuy.quantity -= order.quantity;
				buyOrdersMaxHeap.push(matchedBuy);
				matchedOrders.emplace_back(matchedBuy.price, order.quantity, matchedBuy.timestamp);
				order.quantity = 0;
				break;
			}
			else {
				order.quantity -= matchedBuy.quantity;
				matchedOrders.push_back(std::move(matchedBuy));
			}
		}
		if (order.quantity > 0) {
			sellOrdersMinHeap.push(std::move(order));
		}
		return matchedOrders;
	}

	std::optional<Order> OrderBookHeapImpl::getBestBidOrder() const {
		if (buyOrdersMaxHeap.empty()) {
			return {};
		}
		return buyOrdersMaxHeap.top();
	}

	std::optional<Order> OrderBookHeapImpl::getBestAskOrder() const {
		if (sellOrdersMinHeap.empty()) {
			return {};
		}
		return sellOrdersMinHeap.top();
	}

	size_t OrderBookHeapImpl::getQuantityAtBidPrice(const size_t price) const {
		// TODO
		return 0;
	}

	size_t OrderBookHeapImpl::getQuantityAtAskPrice(const size_t price) const {
		// TODO
		return 0;
	}
}