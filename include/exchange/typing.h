// Copyright 2022, naon

#ifndef INCLUDE_EXCHANGE_TYPING_H_
#define INCLUDE_EXCHANGE_TYPING_H_

#include <cstdint>
#include <string>

#ifdef __GNUC__
#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define LIKELY(x)   x
#define UNLIKELY(x) x
#endif

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
