#include <fibers/sched/suspend.hpp>
#include <fibers/core/fiber.hpp>

namespace concurrency::fibers {

void Suspend(IAwaiter& awaiter) {
  Fiber::Self()->Suspend(awaiter);
}

}  // namespace concurrency::fibers
