// Copyright 2022, naon

#ifndef INCLUDE_EXCHANGE_ORDER_H_
#define INCLUDE_EXCHANGE_ORDER_H_

#include "./typing.h"

namespace exchange {

/**
 * @brief Enumerate the transaction type.
 *
 */
enum class Side {
  kBid,  /* Buying */
  kOffer /* Selling */
};

/// TODO: implement ROD and FOK
/**
 * @brief Enumerate the time in force type.
 *
 */
enum class TimeInForce {
  kIOC,
};

/**
 * @brief The Order structure contains the details of an order including
 * order_id, price and quantity... etc.
 *
 */
struct Order {
 public:
  TimeInForce           time_in_force;
  Typing::OrderIdType   id;
  Side                  side;
  Typing::ProductIdType product;
  Typing::PriceType     price;
  Typing::QuantityType  quantity;
  Typing::TimeType      trade_time;
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_ORDER_H_
