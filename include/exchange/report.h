// Copyright 2022, naon

#ifndef INCLUDE_EXCHANGE_REPORT_H_
#define INCLUDE_EXCHANGE_REPORT_H_

#include "./typing.h"

namespace exchange {

/**
 * @brief The execution report will be sent when an order is executed by the
 * Matcher.
 *
 */
struct ExecutionReport {
  /**
   * @brief Indicate the state of transaction result.
   *
   */
  enum Result {
    kFilled,        /* The order has been filled */
    kPartialFilled, /* The order has been partial filled */
    kCanceled       /* The order has been canceled */
  };

  /// Indicate the state of specified order.
  Result result;
  /// Specify the identifier of order.
  Typing::OrderIdType order_id;
  /// Specify the identifier of product.
  Typing::ProductIdType product_id;
  /// Specify the matched price.
  Typing::PriceType price;
  /// Specify the matched quantity or the canceled quantity.
  Typing::QuantityType quantity;
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_REPORT_H_
