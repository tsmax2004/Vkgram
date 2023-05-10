#include <executors/inline.hpp>

namespace concurrency::executors {

class InlineExecutor : public IExecutor {
 public:
  // IExecutor
  void Submit(IntrusiveTask* task) override {
    task->Run();
  }

  void Submit(IntrusiveTask* task, SchedulerHint) override {
    Submit(task);
  }
};

IExecutor& Inline() {
  static InlineExecutor instance;
  return instance;
}

}  // namespace concurrency::executors
