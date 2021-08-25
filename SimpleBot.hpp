#ifndef SIMPLEBOT_HPP
#define SIMPLEBOT_HPP

#include <memory>
#include <utility>

#include "Algo.hpp"
#include "DvfSimulator.h"
#include "Model.hpp"

// Controller class for Simple bot
class Simplebot {
public:

    // Updates entire application's state, called from main loop
    void Update(Model& model);

private:

    // static utility function for rounding (to fit potential exchange levels)
    [[nodiscard]]
    inline static double RoundToOneDecimal(const double& val) {
        return static_cast<double>(static_cast<int>(val * 10.0)) / 10.0;
    }

    // calculates best bid from the orderbook
    static double BestBid(const IDvfSimulator::OrderBook& orderbook);

    // calculates best ask from the orderbook
    static double BestAsk(const IDvfSimulator::OrderBook& orderbook);

    // updates balances if the orderds were taken
    void UpdateBalances(const double& best_ask, const double& best_bid, Model& model);

    // canceles remaining offers
    void CancelOffers(const double& best_ask, const double& best_bid, Model& model);

    // clean market state
    inline void CleanOffers(const double& best_ask, const double& best_bid, Model& model) {
        UpdateBalances(best_ask, best_bid, model);
        CancelOffers(best_ask, best_bid, model);
    }

    // places new bids
    void PlaceBids(const double& best_ask, const double& best_bid, const TradeSide& side, Model& model);

    // places new asks
    void PlaceAsks(const double& best_ask, const double& best_bid, const TradeSide& side, Model& model);

    // places new offers
    inline void PlaceOffers(const double& best_ask, const double& best_bid, const TradeSide& side, Model& model) {
        PlaceBids(best_ask, best_bid, side, model);
        PlaceAsks(best_ask, best_bid, side, model);
    }

    Algo m_algo;
    std::unique_ptr<IDvfSimulator> m_simulator{ DvfSimulator::Create() };
};

#endif
