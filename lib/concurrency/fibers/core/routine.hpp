#pragma once

#include <function2/function2.hpp>

namespace concurrency::fibers {

using Routine = fu2::unique_function<void()>;

}  // namespace concurrency::fibers
