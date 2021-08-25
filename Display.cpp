#include "Display.hpp"

#include <iostream>

void Display::Print(const Model& model) {
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "AVAILABLE BALANCES:" << std::endl;
    std::cout << "ETH    " << model.GetEthBalance() << std::endl;
    std::cout << "USD    " << model.GetUsdBalance() << std::endl;
    std::cout << std::endl;
    std::cout << "ACTIVE TRADES:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    for(size_t i=0; i<model.GetBids().size(); ++i) {
        std::cout << "buying " << model.GetBids()[i].GetAmount() << " eth at " << model.GetBids()[i].GetPrice() << "$ | ";
        std::cout << "selling " << model.GetAsks()[i].GetAmount() << " eth at " << model.GetAsks()[i].GetPrice() << "$" << std::endl;
    }
    std::cout << std::string(80, '=') << std::endl;
}
