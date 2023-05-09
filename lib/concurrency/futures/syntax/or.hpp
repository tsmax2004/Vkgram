#pragma once

#include <futures/combine/par/first.hpp>

template<typename T>
concurrency::futures::Future<T> operator||(concurrency::futures::Future<T> f,
                                           concurrency::futures::Future<T> g) {
  return concurrency::futures::First(std::move(f), std::move(g));
}
