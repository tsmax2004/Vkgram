#pragma once

#include <futures/make/contract.hpp>
#include <futures/types/future.hpp>
#include <result/types/error.hpp>

namespace concurrency::futures {

template <typename T>
Future<T> Failure(Error err) {
  auto [f, p] = Contract<T>();
  std::move(p).SetError(std::move(err));
  return std::move(f);
}

}  // namespace concurrency::futures
