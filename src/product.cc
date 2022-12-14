// Copyright 2022, naon

#include <exchange/product.h>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <map>

namespace exchange {

namespace detail {

struct Quote {
  Typing::PriceType    bid_price;
  Typing::PriceType    offer_price;
  Typing::QuantityType bid_quantity;
  Typing::QuantityType offer_quantity;
};
/// Use ordered-map since the lower_bound and upper_bound like function is
/// required.
using QuoteBoard = std::map<Typing::TimeType, Quote>;

}  // namespace detail

class Product::Opaque {
 public:
  detail::QuoteBoard           quote_board_;
  bool                         set_started = false;
  detail::QuoteBoard::iterator quote_it_;
};

Product::Product() noexcept : opaque_{std::make_unique<Opaque>()} {
  assert(nullptr != opaque_);
}

Product::~Product() noexcept {}

bool Product::Insert(Typing::TimeType time, std::string_view bid_quote,
                     std::string_view offer_quote) noexcept {
  if (UNLIKELY(nullptr == opaque_ || bid_quote.empty() ||
               offer_quote.empty())) {
    return false;
  }
  [[maybe_unused]] auto [it, suc] =
      opaque_->quote_board_.emplace(time, detail::Quote{});

  if (LIKELY(suc)) {
    /// TODO: exceptions handle
    auto &quote = it->second;
    std::sscanf(bid_quote.data(), "%ld@%f", &quote.bid_quantity,
                &quote.bid_price);
    std::sscanf(offer_quote.data(), "%ld@%f", &quote.offer_quantity,
                &quote.offer_price);
  }

  return suc;
}

void Product::SetTimestamp(Typing::TimeType time) noexcept {
  if (LIKELY(nullptr != opaque_)) {
    auto find = opaque_->quote_board_.upper_bound(time);
    if (find != opaque_->quote_board_.begin()) {
      opaque_->set_started = true;
      opaque_->quote_it_   = --find;
    } else {
      opaque_->set_started = false;
    }
  }
}

Typing::TimeType Product::Next() const noexcept {
  if (LIKELY(nullptr != opaque_)) {
    if (!opaque_->set_started) {
      return opaque_->quote_board_.begin()->first;
    } else {
      auto next = opaque_->quote_it_;
      return (++next)->first;
    }
  }
  return 0;
}

Typing::TimeType Product::Iterate() noexcept {
  if (UNLIKELY(nullptr == opaque_)) {
    return 0;
  }
  /// modify iterations
  if (!opaque_->set_started) {
    opaque_->set_started = true;
    opaque_->quote_it_   = opaque_->quote_board_.begin();
  } else {
    ++opaque_->quote_it_;
  }
  /// calculate the next timestamp
  auto next = opaque_->quote_it_;
  return (++next)->first;
}

std::pair<Typing::QuantityType, Typing::PriceType> Product::TryMatch(
    Order const &order) const noexcept {
  if (UNLIKELY(nullptr == opaque_ || !opaque_->set_started)) {
    return std::make_pair(0, 0.0);
  }
  /// Try to match the order but not effect on map
  auto &quote = opaque_->quote_it_->second;
  if (order.side == Side::kBid && order.price >= quote.offer_price) {
    return std::make_pair(std::min(quote.offer_quantity, order.quantity),
                          quote.offer_price);
  } else if (order.side == Side::kOffer && order.price <= quote.bid_price) {
    return std::make_pair(std::min(quote.bid_quantity, order.quantity),
                          quote.bid_price);
  }
  return std::make_pair(0, 0.0);
}

void Product::Match(Order const &order) noexcept {
  if (UNLIKELY(nullptr == opaque_ || !opaque_->set_started)) {
    return;
  }
  /// match the order and actually effect on map
  auto &quote = opaque_->quote_it_->second;
  if (order.side == Side::kBid && order.price >= quote.offer_price) {
    quote.offer_quantity -= std::min(quote.offer_quantity, order.quantity);
  } else if (order.side == Side::kOffer && order.price <= quote.bid_price) {
    quote.bid_quantity -= std::min(quote.bid_quantity, order.quantity);
  }
}

}  // namespace exchange
