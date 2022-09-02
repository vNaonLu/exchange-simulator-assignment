// Copyright 2022, naon

#ifndef INCLUDE_EXCHANGE_TYPING_H_
#define INCLUDE_EXCHANGE_TYPING_H_

#include <cstdint>
#include <string>

namespace exchange {

/**
 * @brief The typing collections.
 *
 */
struct Typing {
  using TimeType      = unsigned long long;  // NOLINT [runtime/int]
  using ProductIdType = std::string;
  using OrderIdType   = unsigned long long;  // NOLINT [runtime/int]
  using PriceType     = float;
  using QuantityType  = signed long;  // NOLINT [runtime/int]
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_TYPING_H_
