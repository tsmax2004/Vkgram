#pragma once

#include <executors/tp/fast/thread_pool.hpp>
#include <executors/tp/compute//thread_pool.hpp>

namespace concurrency::executors {

// Default thread pool implementation
using ThreadPoolFast = tp::fast::ThreadPool;
using ThreadPoolCompute = tp::compute::ThreadPool;

}  // namespace concurrency::executors
