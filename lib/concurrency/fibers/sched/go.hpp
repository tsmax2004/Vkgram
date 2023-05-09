#pragma once

#include <fibers/core/routine.hpp>
#include <fibers/core/scheduler.hpp>

namespace concurrency::fibers {

// Considered harmful

void Go(Scheduler&, Routine);

void Go(Routine);

}  // namespace concurrency::fibers
