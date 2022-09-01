// Copyright 2022, naon

#ifndef INCLUDE_EXCHANGE_MATCHER_H_
#define INCLUDE_EXCHANGE_MATCHER_H_

#include <functional>
#include <memory>

#include "./exchange.h"
#include "./order.h"
#include "./report.h"

namespace exchange {

/**
 * @brief The matcher is used to match the order transaction according to the
 * product quote in the exchange.
 *
 */
class Matcher {
 private:
  class Opaque;
  std::unique_ptr<Opaque> opaque_;

 public:
  using MatchCallback = std::function<void(ExecutionReport const &)>;

  /**
   * @brief Construct a new Matcher object.
   *
   * @param exchange Specify the target exchange object.
   */
  explicit Matcher(Exchange *exchange) noexcept;

  /**
   * @brief Destroy the Matcher object.
   *
   */
  ~Matcher() noexcept;

  /**
   * @brief Insert an order in to matcher service.
   *
   * @param order Specify the target order.
   * @return Indicate whether the insert operation is success.
   */
  bool InsertOrder(Order const &order) noexcept;

  /**
   * @brief Blocking start to execute orders in exchange,
   *
   * @param callback Specify the callback to handle execution reports.
   */
  void Execute(MatchCallback callback) noexcept;
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_MATCHER_H_
