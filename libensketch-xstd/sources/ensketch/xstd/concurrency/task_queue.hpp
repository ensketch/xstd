#pragma once
#include <ensketch/xstd/concurrency/defaults.hpp>
//
#include <queue>

namespace ensketch::xstd {

/// The `task_queue` class is a thread-safe queue of tasks.
/// Multiple threads are allowed to push new tasks to the queue.
/// Multiple threads are allowed to process tasks from the queue.
/// All given tasks are either seen as fire-and-forget (`push_and_discard`)
/// tasks without any return value or packaged by `std::packaged_task`
/// that returns its respective `std::future` to allow for synchronization.
///
class task_queue {
 public:
  /// The specific task type that is used to store tasks in the queue.
  ///
  using task_type = std::move_only_function<void()>;

  /// The queue container type used to store all tasks.
  /// The `std::queue` container adaptor uses `std::deque` be default.
  ///
  using queue_type = std::queue<task_type>;

  /// Push a task with no return value to the queue
  /// by using fire-and-forget semantic.
  ///
  void push_and_discard(nullary_task_for<void> auto&& task) {
    std::scoped_lock lock{mutex};
    tasks.emplace(std::forward<decltype(task)>(task));
  }

  /// Push a task with return value to the queue by using
  /// fire-and-forget semantic and discard its return value.
  ///
  void push_and_discard(nullary_task auto&& f) {
    push_and_discard([task = std::forward<decltype(f)>(f)](this auto&& self) {
      // Discard the return value.
      // The compiler should warn about `nodiscard` functions.
      std::invoke(std::forward_like<decltype(self)>(task));
    });
  }

  /// Push an arbitrary task to the queue and receive a `std::future`
  /// to its wrapping `std::package_task` for synchronization.
  /// The result is not allowed to be discarded.
  ///
  [[nodiscard]] auto push(nullary_task auto&& f) {
    std::packaged_task task{std::forward<decltype(f)>(f)};
    auto result = task.get_future();
    push_and_discard(std::move(task));
    return result;
  }

  /// Return `false` if the queue is empty.
  /// Otherwise, pop the next task from the queue,
  /// invoke it on the current thread, and return `true`.
  ///
  bool process() {
    task_type task{};
    {
      std::scoped_lock lock{mutex};
      if (tasks.empty()) return false;
      task = move(tasks.front());
      tasks.pop();
    }
    std::invoke(task);
    return true;
  }

  /// Process all available tasks in the queue on the current thread.
  ///
  void process_all() { while (process()); }

 private:
  // Data Members
  //
  queue_type tasks{};          // Queue that contains all tasks.
  mutable std::mutex mutex{};  // Mutual exclusion for thread-safety.
};

}  // namespace ensketch::xstd
