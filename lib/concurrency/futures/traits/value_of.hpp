#pragma once

#include <futures/types/future.hpp>

namespace concurrency::futures::traits {

namespace match {

template <typename T>
struct ValueOf {};

template <typename T>
struct ValueOf<Future<T>> {
  using Type = T;
};

}  // namespace match

template <typename Future>
using ValueOf = typename match::ValueOf<Future>::Type;

}  // namespace concurrency::futures::traits
