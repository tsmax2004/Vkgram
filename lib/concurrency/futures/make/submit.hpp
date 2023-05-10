#pragma once

#include <futures/types/future.hpp>
#include <executors/executor.hpp>
#include <executors/submit.hpp>

#include <futures/make/contract.hpp>

#include <result/traits/value_of.hpp>

#include <type_traits>

namespace concurrency::futures {

namespace traits {

template <typename F>
using SubmitT = result::traits::ValueOf<std::invoke_result_t<F>>;

}  // namespace traits

template <typename F>
Future<traits::SubmitT<F>> Submit(executors::IExecutor& exe, F fun) {
  auto [f, p] = Contract<traits::SubmitT<F>>();

  Submit(exe, [p = std::move(p), fun = std::move(fun)]() mutable {
    std::move(p).Set(fun());
  });

  return std::move(f);
}

}  // namespace concurrency::futures
