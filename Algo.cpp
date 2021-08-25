#include "Algo.hpp"

TradeSide Algo::Update(const double& best_ask, const double& best_bid) {

    double proxy_price = (best_bid + best_ask) / 2.0;

    // initiate to initial proxy price
    if(m_ema < 0.0) {
        m_ema = proxy_price;
    }

    // calculate next ema
    m_ema = (proxy_price * K) + (m_ema * (1-K));

    // return
    if(m_ema < proxy_price) {
        return TradeSide::Buy;
    }
    if(m_ema > proxy_price) {
        return TradeSide::Sell;
    }
    return TradeSide::Stay;
}
