#pragma once

#include <cstdlib>
#include <fibers/sync/event.hpp>

namespace concurrency::fibers {

// https://gobyexample.com/waitgroups

class WaitGroup {
 public:
  void Add(size_t count) {
    counter_.fetch_add(count);
  }

  void Done() {
    if (counter_.fetch_sub(1) == 1) {
      all_done_event_.Fire();
    }
  }

  void Wait() {
    all_done_event_.Wait();
  }

 private:
  twist::ed::stdlike::atomic<size_t> counter_{0};
  Event all_done_event_;
};

}  // namespace concurrency::fibers
