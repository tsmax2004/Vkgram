#pragma once

#include <tl/expected.hpp>

#include <result/types/error.hpp>

namespace concurrency {

// https://github.com/TartanLlama/expected

template <typename T>
using Result = tl::expected<T, Error>;

}  // namespace concurrency
