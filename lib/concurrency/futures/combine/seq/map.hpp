#pragma once

#include <futures/syntax/pipe.hpp>

#include <result/types/result.hpp>
#include <futures/callback/callback.hpp>
#include <futures/make/contract.hpp>

#include <type_traits>

namespace concurrency::futures {

namespace pipe {

template <typename F>
struct [[nodiscard]] Map {
  F fun;

  explicit Map(F f)
      : fun(std::move(f)) {
  }

  template <typename T>
  using U = std::invoke_result_t<F, T>;

  template <typename T>
  Future<U<T>> Pipe(Future<T> producer) {
    auto [consumer, promise] = Contract<U<T>>(producer.GetExecutor());

    producer.Consume([promise = std::move(promise),
                      fun = std::move(fun)](Result<T> result) mutable {
      std::move(promise).Set(result.map(std::move(fun)));
    });

    return std::move(consumer);
  }
};

}  // namespace pipe

// Future<T> -> (T -> U) -> Future<U>

template <typename F>
auto Map(F fun) {
  return pipe::Map{std::move(fun)};
}

}  // namespace concurrency::futures
