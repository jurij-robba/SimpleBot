#ifndef ALGO_HPP
#define ALGO_HPP

#include <utility>

#include "TradeSide.hpp"

/*
Trading algorithm deciding what we believe will
happen in a market. Using simple EMA
*/
class Algo {

    // smothing
    constexpr static double SMOOTHING = 2.0;

    // length (in time units)
    constexpr static double LENGTH = 7.0;

    // factor for calculation
    constexpr static double K = SMOOTHING / (1+LENGTH);

public:

    // Updates the state of market and returns decision
    TradeSide Update(const double& best_ask, const double& best_bid);

private:

    // initiated to -1, then set to current proxy price on first calculation
    double m_ema = -1.0;
};

#endif
