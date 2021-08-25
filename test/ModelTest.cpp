#include "../Model.hpp"

#include "catch.hpp"

TEST_CASE( "order accessors", "[Model::Order]" ) {
    Model::Order trade;

    // id
    REQUIRE(!trade.IsActive());
    trade.SetOrderId(12);
    REQUIRE(trade.IsActive());
    trade.SetOrderId(std::nullopt);
    REQUIRE(!trade.IsActive());

    // price
    REQUIRE(trade.GetPrice() == Approx(0.0));
    trade.SetPrice(12.2);
    REQUIRE(trade.GetPrice() == Approx(12.2));

    // amount
    REQUIRE(trade.GetAmount() == Approx(0.0));
    trade.SetAmount(5.6);
    REQUIRE(trade.GetAmount() == Approx(5.6));
}

TEST_CASE( "model accessors", "[Model]" ) {
    Model model;

    // price
    REQUIRE(model.GetEthBalance() == Approx(10.0));
    model.SetEthBalance(12.2);
    REQUIRE(model.GetEthBalance() == Approx(12.2));

    // amount
    REQUIRE(model.GetUsdBalance() == Approx(2000.0));
    model.SetUsdBalance(1200.0);
    REQUIRE(model.GetUsdBalance() == Approx(1200.0));
}
