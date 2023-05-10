#pragma once

#include <futures/make/value.hpp>
#include <futures/types/future.hpp>
#include <result/types/unit.hpp>

namespace concurrency::futures {

inline Future<Unit> Just() {
  return Value(Unit());
}

}  // namespace concurrency::futures
