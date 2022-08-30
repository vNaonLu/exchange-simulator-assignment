// Copyright 2022, naon

#ifndef INCLUDE_EXCHANGE_PRODUCT_H_
#define INCLUDE_EXCHANGE_PRODUCT_H_

#include <memory>
#include <string_view>

#include "./order.h"
#include "./typing.h"

namespace exchange {

/**
 * @brief The Product is a structure which stores the transactions at the
 * certain time in the exchange and provides the quote search feature.
 *
 */
class Product {
 private:
  class Opaque;
  std::unique_ptr<Opaque> opaque_;

 public:
  /**
   * @brief Construct a new Product object.
   *
   */
  Product() noexcept;

  /**
   * @brief Destroy the Product object.
   *
   */
  ~Product() noexcept;

  /**
   * @brief Save a quote data into its database.
   *
   * @param time Specify timing about this quote.
   * @param bid_quote Specify buying information string.
   * @param offer_quote Specify selling information string.
   * @return Indicate whether the inset operation is success.
   */
  bool Insert(Typing::TimeType time, std::string_view bid_quote,
              std::string_view offer_quote) noexcept;

  /**
   * @brief Iterate the quote by specified time. Note that it will iterate to
   * the nearest time which is less or equal to the target timestamp.
   *
   * @param time Specify the target timestamp.
   */
  void SetTimestamp(Typing::TimeType time) noexcept;

  /**
   * @brief Obtain the next timestamp.
   *
   * @return The next timestamp.
   */
  Typing::TimeType Next() const noexcept;

  /**
   * @brief Iterate to the next timestamp.
   *
   * @return The next timestamp after iterated.
   */
  Typing::TimeType Iterate() noexcept;

  /**
   * @brief Try to match the order and will not actually perform at the
   * database.
   *
   * @param order Specify the target order.
   * @return The predict quantity if the order be matched.
   */
  Typing::QuantityType TryMatch(Order const &order) const noexcept;

  /**
   * @brief Match the order.
   *
   * @param order Specify the target order.
   */
  void Match(Order const &order) noexcept;
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_PRODUCT_H_
