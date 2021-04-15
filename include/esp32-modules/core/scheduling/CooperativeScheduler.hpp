/**
 * @file Scheduler.hpp
 * @author Joschka Seydell (joschka@seydell.org)
 * @brief Provides a wrapper for the TaskScheduler easing up its use.
 * @version 0.1
 * @date 2021-02-19
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef ESP32MODULES__CORE_COOPERATIVESCHEDULER_HPP_
#define ESP32MODULES__CORE_COOPERATIVESCHEDULER_HPP_

// Standard header
#include <cstdint>
#include <map>
#include <memory>

// Preprocessor configuration for the TaskScheduler
// Enable 1 ms powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_TIMEOUT
#define _TASK_WDT_IDS
// Use std::function for task callbacks (instead of function pointers)
#define _TASK_STD_FUNCTION
// Do not use microsecond precision. If needed, compile the project with
// #define _TASK_MICRO_RES

// Third-party header
#include <TaskSchedulerDeclarations.h>  // Forward declarations, includes std::function

namespace Esp32Modules::Core::Scheduling
{
/**
 * @brief Alias for duration values used for task timing.
 *
 * Use the helpers provided by the TaskScheduler library to determine the order of magnitudes:
 *   TASK_MILLISECOND
 *   TASK_SECOND
 *   TASK_MINUTE
 *   TASK_HOUR
 */
using TaskDuration = uint32_t;
using TaskId = uint16_t;  //!< Id of a task used for removing running tasks.

constexpr TaskId INVALID_TASKID{0};  //!< Indicates a task is not valid.
constexpr TaskDuration DEFAULT_TIMEOUT{
    100 * TASK_MILLISECOND};  //!< Default timeout of tasks (used to prevent longrunning tasks.)

/**
 * @brief Provides a std::function based scheduler including resource management.
 *
 * The cooperative scheduler uses the TaskScheduler for task execution and manages task lifetime
 * (persistence, aborting, garbage collection). It is meant to be run as the single active "thread"
 * of the application, i.e. tasks run by the scheduler are never executed in parallel.
 *
 * As at least one task shall be executed (scheduling makes no sense otherwise), a main task must be
 * defined during construction already. The main task or any other tasks are free to add new tasks
 * or to remove existing tasks as the scheduler will make sure to properly serialize task execution.
 *
 * Examples for a main task are implementations checking external interfaces or certain conditions
 * for aborting the overall execution.
 *
 */
class CooperativeScheduler
{
 public:
  /**
   * @brief Initializes the scheduler and starts both the main task and the automatic garbage
   * collection.
   *
   * @param mainTask Main application task to be executed.
   * @param mainInterval Interval in with the main task shall be executed.
   */
  CooperativeScheduler(const TaskCallback& mainTask, const TaskDuration mainInterval);
  /**
   * @brief Properly cleans up everything before shutting down.
   */
  ~CooperativeScheduler();

  /**
   * @brief Adds a one shot task to be executed after the given delay.
   *
   * The resources of the task will automatically be freed by the garbage collection after it
   * finished.
   *
   * @param delay Time after which the task shall be executed.
   * @param task Callback to be executed.
   * @param timeout Timeout after which task execution shall be aborted.
   * @return TaskId Id of the created task - INVALID_TASKID if the task could not be created.
   */
  TaskId AddOneShotTask(const TaskDuration delay, const TaskCallback& task,
                        const TaskDuration timeout = DEFAULT_TIMEOUT);

  /**
   * @brief Adds a cyclic task reoccurring in the given interval.
   *
   * @param interval Time between each execution of the task.
   * @param task Callback to be executed.
   * @param timeout Timeout after which task execution shall be aborted.
   * @return TaskId Id of the created task - INVALID_TASKID if the task could not be created.
   */
  TaskId AddCyclicTask(const TaskDuration interval, const TaskCallback& task,
                       const TaskDuration timeout = DEFAULT_TIMEOUT);

  /**
   * @brief Result of a single scheduler execution cycle.
   */
  enum class ExecutionResult
  {
    OK,       //!< At least one task was executed.
    IDLE,     //!< No tasks were executed.
    ERR_INIT  //!< Scheduler not properly initialized.
  };

  /**
   * @brief Runs the scheduler for one execution cycle.
   *
   * @return ExecutionResult Result of the execution cycle.
   */
  ExecutionResult ExecuteNext();

  /**
   * @brief Abort the given task.
   *
   * The resources of the task will automatically be freed by the garbage collection.
   *
   * @param id Identifies the task to be aborted.
   * @return true if the task was found and aborted, false otherwise.
   */
  bool AbortTask(const TaskId id);

  /**
   * @brief Aborts all currently active tasks (including the main task and garbage collection)
   *
   * @note While this also aborts the main task and garbage collection, they will never be removed
   * from the scheduler during its lifetime so that they can be restarted by running RestartMainTask
   * or RestartAllTasks.
   */
  void AbortAllTasks();

  /**
   * @brief Restarts the main task after it has been aborted with AbortAllTasks.
   */
  void RestartMainTask();

  /**
   * @brief Restarts the tasks that were running before AbortAllTasks was called.
   *
   * @note Only meant to be called after AbortAllTasks. Otherwise, funny behavior might follow e.g.
   * if single tasks have been aborted but not yet collected by garbage collection.
   *
   */
  void RestartAllTasks();

 private:
  std::unique_ptr<Scheduler> mScheduler;           //!< Actual scheduler
  std::map<TaskId, std::unique_ptr<Task>> mTasks;  //!< Persistence for active tasks.
  std::vector<TaskId> mDisabledTasks;  //!< List of diabled tasks queued for garbage collection.

  Task mMainTask;               //!< Main application task.
  Task mGarbageCollectionTask;  //!< Garbage collection.

  /**
   * @brief Types of tasks currently supported.
   */
  enum class TaskType
  {
    ONE_SHOT = TASK_ONCE,  //!< Task running only once, then getting removed.
    CYCLIC = TASK_FOREVER  //!< Continuously running task.
  };

  /**
   * @brief Adds any of the supported tasks to the scheduler queue.
   *
   * @param type Type of task to be added.
   * @param timespan Time after which the one and only or the next execution shall happen.
   * @param timeout Timeout after which task execution shall be aborted.
   * @param task Actual callback to be executed.
   * @return TaskId Id of the created task - INVALID_TASKID if the task could not be created.
   */
  TaskId AddTask(const TaskType type, const TaskDuration timespan, const TaskDuration timeout,
                 const TaskCallback& task);

  /**
   * @brief Marks the task as disabled so that garbage collection can pick it up.
   */
  void MarkTaskAsDisabled();

  /**
   * @brief Removes any tasks marked as disabled (except the main task and the garbage collection).
   */
  void RemoveDisabledTasks();
};
}  // namespace Esp32Modules::Core::Scheduling

#endif  // ESP32MODULES__CORE_COOPERATIVESCHEDULER_HPP_