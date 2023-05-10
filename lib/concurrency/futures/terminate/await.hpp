#pragma once

#include <optional>

#include <twist/ed/stdlike/atomic.hpp>
#include <twist/ed/wait/sys.hpp>

#include <futures/syntax/pipe.hpp>

#include <result/types/result.hpp>

#include <fibers/sync/event.hpp>

namespace concurrency::futures {

namespace pipe {

struct [[nodiscard]] Await {
  template<typename T>
  Result<T> Pipe(Future<T> future) {
    fibers::Event is_ready;
    std::optional<Result<T>> result;

    future.Via(executors::Inline());
    future.Consume([&](Result<T> f_r) {
      result.emplace(std::move(f_r));
      is_ready.Fire();
    });

    is_ready.Wait();
    return std::move(result.value());
  }
};

}  // namespace pipe

inline auto Await() {
  return pipe::Await{};
}

}  // namespace concurrency::futures
