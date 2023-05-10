#pragma once

#include <executors/executor.hpp>

#include <executors/tp/fast/queues/global_queue.hpp>
#include <executors/tp/fast/worker.hpp>
#include <executors/tp/fast/coordinator.hpp>

#include <twist/ed/stdlike/atomic.hpp>

// random_device
#include <twist/ed/stdlike/random.hpp>

#include <deque>

namespace concurrency::executors::tp::fast {

// Scalable work-stealing scheduler for short-lived tasks

class ThreadPool : public IExecutor {
  friend class Worker;

 public:
  explicit ThreadPool(size_t threads);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  void Start();

  // IExecutor
  void Submit(IntrusiveTask*) override;
  void Submit(IntrusiveTask*, SchedulerHint) override;

  void WaitIdle();
  void Stop();

  static ThreadPool* Current();

 private:
  bool MyWorker(Worker*);

  const size_t threads_;
  std::deque<Worker> workers_;
  Coordinator coordinator_;
  GlobalQueue global_tasks_;
  twist::ed::stdlike::random_device random_;
  twist::ed::stdlike::atomic<bool> is_stopped_{false};
};

}  // namespace concurrency::executors::tp::fast
