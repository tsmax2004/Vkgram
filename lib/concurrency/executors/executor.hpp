#pragma once

#include <executors/task.hpp>
#include <executors/hint.hpp>

namespace concurrency::executors {

struct IExecutor {
  virtual ~IExecutor() = default;

  virtual void Submit(IntrusiveTask* task) = 0;
  virtual void Submit(IntrusiveTask* task, SchedulerHint hint) = 0;
};

}  // namespace concurrency::executors
