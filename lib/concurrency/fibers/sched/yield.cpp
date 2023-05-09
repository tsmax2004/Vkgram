#include <fibers/sched/yield.hpp>
#include <fibers/sched/suspend.hpp>

#include <fibers/core/fiber.hpp>
#include <fibers/core/awaiter.hpp>

namespace concurrency::fibers {

struct YieldAwaiter : IAwaiter {
 public:
  bool AwaitSuspend(FiberHandle fiber) override {
    fiber.Schedule(executors::SchedulerHint::ToOtherThread);
    return true;
  }
};

void Yield() {
  YieldAwaiter awaiter;
  Suspend(awaiter);
}

}  // namespace concurrency::fibers
