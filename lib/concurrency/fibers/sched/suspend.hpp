#pragma once

#include <fibers/core/awaiter.hpp>

namespace concurrency::fibers {

void Suspend(IAwaiter& awaiter);

}  // namespace concurrency::fibers
