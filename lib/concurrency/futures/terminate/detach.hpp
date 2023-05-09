#pragma once

#include <futures/syntax/pipe.hpp>
#include <futures/callback/callback.hpp>

namespace concurrency::futures {

namespace pipe {

struct [[nodiscard]] Detach {
  template <typename T>
  void Pipe(Future<T> f) {
    f.Via(executors::Inline());
    f.Consume([](Result<T>) {});
  }
};

}  // namespace pipe

inline auto Detach() {
  return pipe::Detach{};
}

}  // namespace concurrency::futures
