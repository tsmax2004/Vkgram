#include <fibers/sched/go.hpp>

#include <fibers/core/fiber.hpp>

namespace concurrency::fibers {

void Go(Scheduler& scheduler, Routine routine) {
  auto fiber = new Fiber(scheduler, std::move(routine));
  fiber->Schedule();
}

void Go(Routine routine) {
  Go(Fiber::Self()->scheduler_, std::move(routine));
}

}  // namespace concurrency::fibers
