#pragma once

#include <futures/combine/seq/map.hpp>
#include <futures/combine/seq/flatten.hpp>
#include <futures/combine/seq/via.hpp>

#include <futures/syntax/pipe.hpp>

#include <futures/traits/value_of.hpp>

#include <type_traits>

namespace concurrency::futures {

namespace pipe {

template <typename F>
struct [[nodiscard]] FlatMap {
  F fun;

  explicit FlatMap(F f)
      : fun(std::move(f)) {
  }

  template <typename T>
  using U = traits::ValueOf<std::invoke_result_t<F, T>>;

  template <typename T>
  Future<U<T>> Pipe(Future<T> future) {
    auto& exe = future.GetExecutor();
    return std::move(future) | Map(std::move(fun)) | Via(executors::Inline()) |
           Flatten() | Via(exe);
  }
};

}  // namespace pipe

// Future<T> -> (T -> Future<U>) -> Future<U>

template <typename F>
auto FlatMap(F fun) {
  return pipe::FlatMap{std::move(fun)};
}

}  // namespace concurrency::futures
