#include "SimpleBot.hpp"

#include <limits>
#include <algorithm>

#include "TradeSide.hpp"

void Simplebot::Update(Model& model) {
    // update market
    IDvfSimulator::OrderBook orderbook = m_simulator->GetOrderBook();
    double best_ask = BestAsk(orderbook);
    double best_bid = BestBid(orderbook);

    // clean offers
    CleanOffers(best_ask, best_bid, model);

    // place new offers
    TradeSide decision = m_algo.Update(best_ask, best_bid);
    PlaceOffers(best_ask, best_bid, decision, model);
}



double Simplebot::BestBid(const IDvfSimulator::OrderBook& orderbook) {
    double best_bid = 0;
    for(auto& level: orderbook) {
        if(level.second > 0.0 && level.first > best_bid) {
            best_bid = level.first;
        }
    }
    return best_bid;
}

double Simplebot::BestAsk(const IDvfSimulator::OrderBook& orderbook) {
    double best_ask = std::numeric_limits<double>::max();
    for(auto& level: orderbook) {
        if(level.second < 0.0 && level.first < best_ask) {
            best_ask = level.first;
        }
    }
    return best_ask;
}

void Simplebot::UpdateBalances(const double& best_ask, const double& best_bid, Model& model) {

    for(auto& bid: model.GetBids()) {
        if(bid.IsActive() && bid.GetPrice() > best_ask) { // should be best bid but api seems to assume best ask
            model.SetEthBalance(model.GetEthBalance() + bid.GetAmount());
            // std::cout << "Finishing bid: " << bid.GetOrderId().value() << "(" << bid.GetPrice() << " " << best_bid << ")" << std::endl;
            bid.SetOrderId(std::nullopt);
        }
    }
    for(auto& ask: model.GetAsks()) {
        if(ask.IsActive() && ask.GetPrice() < best_bid) { // should be best ask but api seems to assume best bid
            model.SetUsdBalance(model.GetUsdBalance() + (ask.GetAmount() * ask.GetPrice()));
            // std::cout << "Finishing ask: " << ask.GetOrderId().value()  << "(" << ask.GetPrice() << " " << best_ask << ")" << std::endl;
            ask.SetOrderId(std::nullopt);
        }
    }
}

void Simplebot::CancelOffers(const double& best_ask, const double& best_bid, Model& model) {
    for(auto& bid: model.GetBids()) {
        if(bid.IsActive()) {
            model.SetUsdBalance(model.GetUsdBalance() + (bid.GetAmount() * bid.GetPrice()));
            // std::cout << "Canceling " << bid.GetOrderId().value() << std::endl;
            m_simulator->CancelOrder(bid.GetOrderId().value());
            bid.SetOrderId(std::nullopt);
        }
    }
    for(auto& ask: model.GetAsks()) {
        if(ask.IsActive()) {
            model.SetEthBalance(model.GetEthBalance() + ask.GetAmount());
            // std::cout << "Canceling " << ask.GetOrderId().value() << std::endl;
            m_simulator->CancelOrder(ask.GetOrderId().value());
            ask.SetOrderId(std::nullopt);
        }
    }

}

void Simplebot::PlaceBids(const double& best_ask, const double& best_bid, const TradeSide& side, Model& model) {
    double min_price;
    double max_price;
    double amount_usd;

    // we believe market is going up, we use half of budget and place our offers above existing bids
    if(side == TradeSide::Buy) {
        max_price = std::min(best_ask, 1.05*best_bid);
        min_price = best_bid;
        amount_usd = 0.1*model.GetUsdBalance();
    }
    // we believe market is going down so we let others take the hit and use only quarter of our budget
    else {
        max_price = best_bid;
        min_price = 0.95 * best_bid;
        amount_usd = 0.05*model.GetUsdBalance();
    }
    // place orders
    double step = (max_price-min_price) / 5.0;
    for(size_t i = 0; i<5; ++i) {
        double price = RoundToOneDecimal(min_price + static_cast<double>(i)*step);
        double amount = RoundToOneDecimal(amount_usd / price);
        model.GetBids()[i].SetOrderId(m_simulator->PlaceOrder(price, amount));
        if(model.GetBids()[i].IsActive()) {
            model.SetUsdBalance(model.GetUsdBalance() - (price * amount));
            model.GetBids()[i].SetPrice(price);
            model.GetBids()[i].SetAmount(amount);
        }
    }
}

void Simplebot::PlaceAsks(const double& best_ask, const double& best_bid, const TradeSide& side, Model& model) {

    double min_price;
    double max_price;
    double amount_eth;

    // we believe market is going down, we use half of budget and place our offers below existing asks
    if(side == TradeSide::Sell) {
        min_price = std::max(best_bid, 0.95*best_ask);
        max_price = best_ask;
        amount_eth = 0.1*model.GetEthBalance();
    }
    // we believe market is going down so we let others take the hit and use only quarter of our budget
    else {
        max_price = 1.05 * best_ask;
        min_price = best_ask;
        amount_eth = 0.05 * model.GetEthBalance();
    }
    // place orders
    double step = (max_price-min_price) / 5.0;
    for(size_t i = 0; i<5; ++i) {
        double price = RoundToOneDecimal(min_price + static_cast<double>(i)*step);
        double amount = RoundToOneDecimal(amount_eth);
        model.GetAsks()[i].SetOrderId(m_simulator->PlaceOrder(price, -amount));
        if(model.GetAsks()[i].IsActive()) {
            model.SetEthBalance(model.GetEthBalance() - amount);
            model.GetAsks()[i].SetPrice(price);
            model.GetAsks()[i].SetAmount(amount);
        }
    }
}

