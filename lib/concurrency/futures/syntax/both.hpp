#pragma once

#include <futures/combine/par/all.hpp>

template <typename X, typename Y>
auto operator+(concurrency::futures::Future<X> f, concurrency::futures::Future<Y> g) {
  return concurrency::futures::Both(std::move(f), std::move(g));
}
