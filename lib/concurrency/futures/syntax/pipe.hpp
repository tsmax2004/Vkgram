#pragma once

#include <futures/types/future.hpp>

template <typename T, typename C>
auto operator|(concurrency::futures::Future<T> f, C c) {
  return c.Pipe(std::move(f));
}
