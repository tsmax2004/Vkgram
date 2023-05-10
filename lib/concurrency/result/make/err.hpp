#pragma once

#include <result/types/result.hpp>

namespace concurrency::result {

/*
 *
 * Usage:
 *
 * futures::Just() | futures::AndThen([](Unit) -> Result<int> {
 *   return result::Err(Timeout());
 * });
 *
 */

inline auto Err(Error error) {
  return tl::unexpected(error);
}

}  // namespace concurrency::result
