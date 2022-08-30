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
class Order {
 public:
  TimeInForce const         time_in_force;
  Typing::OrderIdType const id;
  Typing::PriceType         price;
  Typing::QuantityType      quantity;

 public:
  /**
   * @brief Construct a new Order object. Note for the arguments are the
   * constant properties of an order so the order_id and time_in_force flag are
   * not mutable.
   *
   * @param order_id Specify the target id for this order.
   * @param tif Specify the time in force flag for this order.
   */
  Order(Typing::OrderIdType const &order_id, TimeInForce tif) noexcept
      : time_in_force{tif}, id{order_id} {}

  ~Order() = default;
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_ORDER_H_
