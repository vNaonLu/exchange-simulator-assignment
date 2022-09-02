// Copyright 2022, naon

#include <exchange/matcher.h>

#include <algorithm>
#include <vector>

namespace exchange {

class Matcher::Opaque {
 public:
  Exchange          *exchange;
  std::vector<Order> orders;
};

Matcher::Matcher(Exchange *exchange) noexcept
    : opaque_{std::make_unique<Opaque>()} {
  if (nullptr != opaque_) {
    opaque_->exchange = exchange;
    opaque_->orders.clear();
  }
}

Matcher::~Matcher() noexcept {}

bool Matcher::InsertOrder(Order const &order) noexcept {
  if (nullptr == opaque_) {
    /// TODO: unlikely
    return false;
  }
  opaque_->orders.emplace_back(order);
  return true;
}

void Matcher::Execute(MatchCallback callback, Typing::TimeType latency) noexcept {
  if (nullptr == callback || nullptr == opaque_) {
    /// TODO: unlikely
    return;
  }
  sort(opaque_->orders.begin(), opaque_->orders.end(),
       [](Order const &o1, Order const &o2) {
         return o1.trade_time < o2.trade_time;
       });

  Typing::ProductIdType *pid     = nullptr;
  Product               *product = nullptr;
  ExecutionReport        report;
  for (auto &order : opaque_->orders) {
    auto current_time = order.trade_time + latency * 1'000;
    report.order_id   = order.id;
    report.product_id = order.product;

    /// If the old product is not the target of current order.
    if (nullptr == pid || nullptr == product || *pid != order.product) {
      product = opaque_->exchange->GetProduct(order.product);
      if (nullptr == product) {
        /// TODO: unlikey
        report.result   = ExecutionReport::kCanceled;
        report.price    = 0.0;
        report.quantity = order.quantity;
        callback(report);
        continue;
      }
      pid = &order.product;
      product->SetTimestamp(current_time);
    }
    /// Iterate to the proper time
    auto next_time = product->Next();
    while (next_time < current_time) {
      next_time = product->Iterate();
    }
    /// Try to match and get the proper result
    auto [mat_qty, mat_prc] = product->TryMatch(order);

    switch (order.time_in_force) {
      case TimeInForce::kIOC:
        if (mat_qty == order.quantity) {
          report.result   = ExecutionReport::kFilled;
          report.quantity = mat_qty;
          report.price    = mat_prc;
          callback(report);
          product->Match(order);
        } else if (mat_qty > 0) {
          report.result   = ExecutionReport::kPartialFilled;
          report.quantity = mat_qty;
          report.price    = mat_prc;
          callback(report);
          product->Match(order);
          /// The left shares are canceled.
          report.result   = ExecutionReport::kCanceled;
          report.quantity = order.quantity - mat_qty;
          report.price    = 0.0;
          callback(report);
        } else {
          /// fail to match the order.
          report.result   = ExecutionReport::kCanceled;
          report.quantity = order.quantity;
          report.price    = 0.0;
          callback(report);
        }
        break;

      default:
        /// Cancel the undefined order.
        report.result   = ExecutionReport::kCanceled;
        report.quantity = order.quantity;
        report.price    = 0.0;
        callback(report);
        break;
    }
  }
}
}  // namespace exchange
