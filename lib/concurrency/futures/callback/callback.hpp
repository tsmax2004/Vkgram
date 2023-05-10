#pragma once

#include <function2/function2.hpp>

#include <result/types/result.hpp>

namespace concurrency::futures::detail {

template <typename T>
using Callback = fu2::function<void(Result<T>)>;

// template <typename T>
// struct Callback {
//  public:
//   virtual void operator()(Result<T>) = 0;
// };

}  // namespace concurrency::futures::detail