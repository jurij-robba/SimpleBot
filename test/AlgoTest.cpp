#include "../Algo.hpp"

#include "catch.hpp"

TEST_CASE( "Buy", "[Algo]" ) {
    Algo algo;
    algo.Update(20, 10);

    // market moved up, we should be buying
    REQUIRE(algo.Update(25, 15) == TradeSide::Buy);
}

TEST_CASE( "Sell", "[Algo]" ) {
    Algo algo;
    algo.Update(20, 10);

    // market moved down, we should be selling
    REQUIRE(algo.Update(15, 5) == TradeSide::Sell);
}
