// Copyright 2022, naon

#include <exchange/exchange.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>

namespace exchange {

namespace detail {
using ProductBoard =
    std::unordered_map<Typing::ProductIdType, std::unique_ptr<Product>>;
}  // namespace detail

class Exchange::Opaque {
 public:
  detail::ProductBoard product_board;
};

Exchange::Exchange() noexcept : opaque_{std::make_unique<Opaque>()} {}

Exchange::~Exchange() noexcept {}

bool Exchange::Load(std::string_view file) noexcept {
  if (UNLIKELY(nullptr == opaque_)) {
    return false;
  }

  std::ifstream f(file.data());
  if (!f.is_open()) {
    return false;
  }

  std::string           data;
  Typing::TimeType      time;
  Typing::ProductIdType id;
  std::string           bid, offer;
  while (f >> time >> id >> bid >> offer) {
    /// TODO: exception handle
    auto product = GetProduct(id);
    if (nullptr == product) {
      auto [it, insert] =
          opaque_->product_board.emplace(id, std::make_unique<Product>());
      if (UNLIKELY(!insert)) {
        return false;
      }
      product = it->second.get();
    }
    if (!product->Insert(time, bid, offer)) {
      return false;
    }
  }

  return true;
}

Product *Exchange::GetProduct(
    Typing::ProductIdType const &product_id) noexcept {
  if (UNLIKELY(nullptr == opaque_)) {
    return nullptr;
  }

  auto find = opaque_->product_board.find(product_id);
  return find == opaque_->product_board.end() ? nullptr : find->second.get();
}

}  // namespace exchange
