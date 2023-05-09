#pragma once

#include <executors/executor.hpp>

namespace concurrency::executors {

// Executes task immediately on the current thread

IExecutor& Inline();

}  // namespace concurrency::executors
