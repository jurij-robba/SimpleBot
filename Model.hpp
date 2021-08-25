#ifndef MODEL_HPP
#define MODEL_HPP

#include <optional>
#include <array>

#include "DvfSimulator.h" // only for types
#include "TradeSide.hpp"

// Model storing current state
class Model {
public:

    // Order class storing all info about an order
    class Order {
    public:

        // If order is active it will have id set
        [[nodiscard]]
        inline bool IsActive() const noexcept {
            return m_order_id.has_value();
        }

        // Order id
        [[nodiscard]]
        inline const std::optional<IDvfSimulator::OrderID>& GetOrderId() const {
            return m_order_id;
        }
        inline void SetOrderId(const std::optional<IDvfSimulator::OrderID>& order_id) {
            m_order_id = order_id;
        }

        // price
        [[nodiscard]]
        inline double GetPrice() const noexcept {
            return m_price;
        }
        inline void SetPrice(const double& price) noexcept {
            m_price = price;
        }

        // amount
        [[nodiscard]]
        inline double GetAmount() const noexcept {
            return m_amount;
        }
        inline void SetAmount(const double& amount) noexcept {
            m_amount = amount;
        }

    private:

        std::optional<IDvfSimulator::OrderID> m_order_id = std::nullopt;
        double m_price = 0;
        double m_amount = 0;
    };

    // Eth Balance
    [[nodiscard]]
    inline double GetEthBalance() const noexcept {
        return m_eth_balance;
    }
    inline void SetEthBalance(const double& balance) noexcept {
        m_eth_balance = balance;
    }

    // Usd balance
    [[nodiscard]]
    inline double GetUsdBalance() const noexcept {
        return m_usd_balance;
    }
    inline void SetUsdBalance(const double& balance) noexcept {
        m_usd_balance = balance;
    }

    // asks
    inline std::array<Order, 5>& GetAsks() { return m_asks; }
    inline const std::array<Order, 5>& GetAsks() const { return m_asks; }

    // bids
    inline std::array<Order, 5>& GetBids() { return m_bids; }
    inline const std::array<Order, 5>& GetBids() const { return m_bids; }

private:

    double m_eth_balance = 10.0;
    double m_usd_balance = 2000.0;
    std::array<Order, 5> m_asks;
    std::array<Order, 5> m_bids;

};

#endif
