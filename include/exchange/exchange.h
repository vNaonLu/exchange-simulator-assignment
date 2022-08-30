// Copyright 2022, naon

#ifndef INCLUDE_EXCHANGE_EXCHANGE_H_
#define INCLUDE_EXCHANGE_EXCHANGE_H_

#include <memory>
#include <string_view>

#include "./product.h"
#include "./typing.h"

namespace exchange {

/**
 * @brief The exchange structure stores the products quote and timestamp.
 *
 */
class Exchange {
 private:
  class Opaque;
  std::unique_ptr<Opaque> opaque_;

 public:
  /**
   * @brief Construct a new Exchange object.
   * 
   */
  Exchange() noexcept;

  /**
   * @brief Destroy the Exchange object.
   * 
   */
  ~Exchange() noexcept;

  /**
   * @brief Load the file from specified file location.
   * 
   * @param file Specify the target file location.
   * @return Indicate whether the load operation is success.
   */
  bool Load(std::string_view file) noexcept;

  /**
   * @brief Obtain the product stored in the database.
   * 
   * @param product_id Specify the unique id of product.
   * @return A pointer to product if found, nullptr otherwise.
   */
  Product *GetProduct(Typing::ProductIdType const &product_id) noexcept;
};

}  // namespace exchange

#endif  // INCLUDE_EXCHANGE_EXCHANGE_H_
