#pragma once

#include <fibers/core/handle.hpp>

namespace concurrency::fibers {

struct IAwaiter {
 public:
  virtual ~IAwaiter() = default;

  virtual bool AwaitSuspend(FiberHandle) = 0;
};

}  // namespace concurrency::fibers
