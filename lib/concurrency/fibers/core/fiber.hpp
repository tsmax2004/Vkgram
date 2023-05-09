#pragma once

#include <optional>

#include <fibers/core/routine.hpp>
#include <fibers/core/scheduler.hpp>
#include <fibers/core/handle.hpp>

#include <fibers/sched/go.hpp>

#include <coro/simple.hpp>

#include <executors/executor.hpp>
#include <executors/hint.hpp>

#include <fibers/core/awaiter.hpp>

namespace concurrency::fibers {

// Fiber = stackful coroutine + scheduler (executor)

class Fiber : executors::IntrusiveTask {
  friend void Go(Scheduler&, Routine);
  friend void Go(Routine);

 public:
  void Suspend(IAwaiter&);

  void Schedule();
  void Schedule(executors::SchedulerHint);
  void Switch();

  // Task
  void Run() noexcept override;

  static Fiber* Self();

 private:
  Fiber(Scheduler&, Routine);

  Scheduler& scheduler_;
  coro::Coroutine coroutine_;
  IAwaiter* awaiter_;
};

}  // namespace concurrency::fibers
