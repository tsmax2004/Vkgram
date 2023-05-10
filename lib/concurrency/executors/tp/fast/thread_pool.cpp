#include <executors/tp/fast/thread_pool.hpp>

namespace concurrency::executors::tp::fast {

ThreadPool::ThreadPool(size_t threads)
    : threads_(threads),
      coordinator_(threads_) {
  for (size_t i = 0; i < threads; ++i) {
    workers_.emplace_back(*this, i);
  }
}

void ThreadPool::Start() {
  for (auto& worker : workers_) {
    worker.Start();
  }
}

ThreadPool::~ThreadPool() {
  WHEELS_ASSERT(is_stopped_.load(), "ThreadPool was not stopped!");
}

void ThreadPool::Submit(TaskBase* task) {
  Submit(task, SchedulerHint::UpToYou);
}

void ThreadPool::Submit(concurrency::executors::IntrusiveTask* task,
                        concurrency::executors::SchedulerHint hint) {
  if (MyWorker(Worker::Current())) {
    Worker::Current()->Push(task, hint);
  } else {
    global_tasks_.Push(task);
  }

  coordinator_.Notify();
}

void ThreadPool::WaitIdle() {
  if (is_stopped_.load()) {
    return;
  }
  coordinator_.WaitIdle();
}

void ThreadPool::Stop() {
  is_stopped_.store(true);

  for (auto& worker : workers_) {
    worker.Stop();
  }

  coordinator_.WakeAll();

  for (auto& worker : workers_) {
    worker.Join();
  }
}

ThreadPool* ThreadPool::Current() {
  if (Worker::Current() == nullptr) {
    return nullptr;
  }
  return &Worker::Current()->Host();
}

bool ThreadPool::MyWorker(Worker* worker) {
  return worker != nullptr && &worker->Host() == this;
}

}  // namespace concurrency::executors::tp::fast
