#pragma once

#include <futures/types/future.hpp>
#include <futures/make/contract.hpp>

namespace concurrency::futures {

template <typename T>
Future<T> Value(T value) {
  auto [f, p] = Contract<T>();
  std::move(p).SetValue(std::forward<T>(value));
  return std::move(f);
}

}  // namespace concurrency::futures
