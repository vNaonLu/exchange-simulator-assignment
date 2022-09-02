# Exchange Simulator Assignment
[![githubbuild](https://github.com/vNaonLu/exchange-simulator-assignment/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/vNaonLu/exchange-simulator-assignment/actions)

This project is a market matching simulation tool. By entering quote data, order data and an optional latency input, the tool can generate a forcasts.

## Getting Started

To get started, see the sections [Requirement](#requirement), [Build](#build) and the [Usage](#usage).

## Requirement

The following minimum verions are required to build the tool.

- C++20 supported compiler (though no C++20 feature are used)
- CMake 3.20 or above
- Unix-like OS

## Build

This describes the build process using cmake. As pre-requisites, you'll need git and cmake installed.

```sh
# Check out the porject.
$ git clone https://github.com/vNaonLu/exchange-simulator-assignment.git assignment
# Go to the project root directory.
$ cd assignment
# Generate build system files with cmake
$ cmake -S . -B "build" -DCMAKE_BUILD_TYPE=Release
# Build the project
$ cmake --build "build"
```

The build directory should now look something like this:

```
+/exchange-simulator
|
+--+/build
|  |
|  +--/exchange-simulator
|  |
...
```

## Usage

There is only one executable `exchange-simulator` in the building directory after building the project. And the basic usage of tool is:

```sh
# in the project root directory
$ ./build/exchange-simulator <latency> <quote_file> <order_file>
```

- the `latency` is in millisecond and is used to simulate the elapsed time of transfer to the exchange.

- the `quote_file` is a file containing a quote information for a sequence of time and the format of `quote_file` looks like:

    ```
    timestamp product_name bid_qty@bid_price offer_qty@offer_price
    timestamp product_name bid_qty@bid_price offer_qty@offer_price
    timestamp product_name bid_qty@bid_price offer_qty@offer_price
    ...
    ``` 
    Note for the `timestamp` is microseconds from UTC of Jan 1, 1970.

- the `order_file` is a file containing a order information for a sequence of time and the format of `order_file` looks like:

    ```
    YYYY-MM-DD HH:mm:ss.sss - [order_id] time_in_force: product side qty@price
    YYYY-MM-DD HH:mm:ss.sss - [order_id] time_in_force: product side qty@price
    YYYY-MM-DD HH:mm:ss.sss - [order_id] time_in_force: product side qty@price
    ...
    ```

    Note that `time_in_force` is only accept `IOCOrder` in the current version and `side` is one of the strings of `BUY` or `SELL`.

A simple output may looks like:

```sh
$ ./build/exchange-simulator 1000 market.txt orders.txt
orderId:  1401534018 [filled] price=2937.000 qty=10
orderId:  1501534018 [canceled]
orderId:  1601534018 [partial filled] price=2935.000 qty=3
orderId:  1601534018 [canceled]
orderId:  1701534018 [filled] price=2934.000 qty=10
orderId:  1801534018 [canceled]
orderId:  1901534018 [filled] price=2927.000 qty=10
...
```