// Copyright 2022, naon

#include <exchange/exchange.h>
#include <exchange/matcher.h>
#include <exchange/order.h>

#include <chrono>  // NOLINT [build/c++11]
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

using exchange::Typing;
using std::cerr;
using std::cout;
using std::endl;

struct Option {
  Typing::TimeType latency;
  std::string_view quote_file;
  std::string_view order_file;
};

void                  Usage() noexcept;
std::optional<Option> Parse(int argc, char **argv) noexcept;
Typing::TimeType      ConvertTime(char const *time) noexcept;

int main(int argc, char **argv) {
  auto opts = Parse(argc, argv);

  if (!opts.has_value()) {
    return EXIT_FAILURE;
  }

  exchange::Exchange exch;
  exchange::Matcher  mat(&exch);
  std::ifstream      ord_file(opts->order_file.data());

  if (!ord_file.is_open()) {
    cerr << "(error) failed to open: " << opts->order_file << endl;
    return EXIT_FAILURE;
  }

  if (!exch.Load(opts->quote_file)) {
    cerr << "(error) failed to parse: " << opts->quote_file << endl;
    return EXIT_FAILURE;
  }

  exchange::Order order;
  char            buf[1024]    = {0};
  char            date[24]     = {0};
  char            tif[20]      = {0};
  char            product[100] = {0};
  char            side[5]      = {0};
  while (ord_file.getline(buf, sizeof(buf))) {
    if (UNLIKELY(EOF == std::sscanf(buf, "%23c - [%llu] %[^:]: %s %s %ld@%f",
                                    date, &order.id, tif, product, side,
                                    &order.quantity, &order.price))) {
      cerr << "(error) failed to parse order file!" << endl;
      return EXIT_FAILURE;
    }

    order.trade_time = ConvertTime(date);
    if (order.trade_time == 0) {
      cerr << "(error) invalid timestamp: " << date << endl;
      return EXIT_FAILURE;
    }

    order.product = product;
    if (std::strcmp(tif, "IOCOrder") == 0) {
      order.time_in_force = exchange::TimeInForce::kIOC;
    } else {
      cerr << "(error) invalid time in force: " << tif << endl;
      return EXIT_FAILURE;
    }

    if (std::strcmp(side, "BUY") == 0) {
      order.side = exchange::Side::kBid;
    } else if (std::strcmp(side, "SELL") == 0) {
      order.side = exchange::Side::kOffer;
    } else {
      cerr << "(error) invalid side: " << side << endl;
      return EXIT_FAILURE;
    }

    mat.InsertOrder(order);
  }

  mat.Execute(
      [](exchange::ExecutionReport const &rep) {
        cout << "orderId:";
        cout << std::setw(12) << std::right << std::setfill(' ') << rep.order_id
             << " ";
        switch (rep.result) {
          case exchange::ExecutionReport::kCanceled:
            cout << "[canceled]";
            break;
          case exchange::ExecutionReport::kFilled:
            cout << "[filled]";
            cout << " price=" << std::fixed << std::setprecision(3)
                 << rep.price;
            cout << " qty=" << rep.quantity;
            break;
          case exchange::ExecutionReport::kPartialFilled:
            cout << "[partial filled]";
            cout << " price=" << std::fixed << std::setprecision(3)
                 << rep.price;
            cout << " qty=" << rep.quantity;
            break;
        }
        cout << endl;
      },
      opts->latency);

  return EXIT_SUCCESS;
}

void Usage() noexcept {
  cout << "Usage: exchange-simulator <latency> <quote> <orders>" << endl;
}

std::optional<Option> Parse(int argc, char **argv) noexcept {
  if (argc < 3) {
    Usage();
    return std::nullopt;
  }

  Option opt;
  opt.quote_file = argv[2];
  opt.order_file = argv[3];
  try {
    opt.latency = std::stoull(argv[1]);
  } catch (const std::exception &e) {
    cerr << "(error) invalid input: " << argv[1] << endl;
  }
  return std::make_optional(std::move(opt));
}

Typing::TimeType ConvertTime(char const *time) noexcept {
  using std::chrono::duration_cast;
  using std::chrono::microseconds;
  using std::chrono::system_clock;
  int     ms;
  std::tm t;
  std::memset(&t, 0, sizeof(t));
  auto res =
      std::sscanf(time, "%4d-%2d-%2d %2d:%2d:%2d.%3d", &t.tm_year, &t.tm_mon,
                  &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec, &ms);

  if (UNLIKELY(res == EOF)) {
    return 0;
  }

  t.tm_year -= 1900;  // year from 1900
  t.tm_mon -= 1;      // month from Jan

  auto             time_point = system_clock::from_time_t(std::mktime(&t));
  Typing::TimeType us_since_epoch =
      duration_cast<microseconds>(time_point.time_since_epoch()).count();
  return us_since_epoch + ms * 1000;  // *1000 to microsecond
}
