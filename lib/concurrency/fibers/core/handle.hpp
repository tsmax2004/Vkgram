#pragma once

#include <fibers/core/fwd.hpp>
#include <executors/hint.hpp>

namespace concurrency::fibers {

// Lightweight non-owning handle to a _suspended_ fiber

class FiberHandle {
  friend class Fiber;

 public:
  FiberHandle()
      : FiberHandle(nullptr) {
  }

  static FiberHandle Invalid() {
    return FiberHandle(nullptr);
  }

  bool IsValid() const {
    return fiber_ != nullptr;
  }

  // Schedule to the associated scheduler
  void Schedule();
  void Schedule(executors::SchedulerHint);

  // Switch to this fiber immediately
  // For symmetric transfer
  void Switch();

 private:
  explicit FiberHandle(Fiber* fiber)
      : fiber_(fiber) {
  }

  Fiber* Release();

 private:
  Fiber* fiber_;
};

}  // namespace concurrency::fibers