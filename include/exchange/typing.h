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
  using TimeType      = uint64_t;
  using ProductIdType = std::string;
  using OrderIdType   = uint64_t;
  using PriceType     = float;
  using QuantityType  = int32_t;
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_TYPING_H_
