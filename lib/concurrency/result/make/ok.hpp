#pragma once

#include <result/types/result.hpp>
#include <result/types/status.hpp>

namespace concurrency::result {

/*
 * Usage:
 *
 * futures::Value(1) | futures::AndThen([](int v) {
 *   return result::Ok(v + 1);
 * })
 *
 */

template <typename T>
inline Result<T> Ok(T value) {
  return {std::move(value)};
}

/*
 * Usage:
 *
 * futures::Just() | futures::AndThen([](Unit) {
 *   return result::Ok();
 * });
 *
 */

inline Status Ok() {
  return {Unit{}};
}

}  // namespace concurrency::result