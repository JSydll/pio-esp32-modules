#include "esp32-modules/core/scheduling/CooperativeScheduler.hpp"

// Now include the TaskScheduler implementation
#include <TaskScheduler.h>

using namespace Esp32Modules::Core::Scheduling;

namespace
{
constexpr TaskDuration GARBAGE_COLLECTION_INTERVAL{
    10 * TASK_SECOND};  //!< Run garbage collection every 10 seconds.
}

CooperativeScheduler::CooperativeScheduler(const TaskCallback& mainTask,
                                           const TaskDuration mainInterval)
    : mScheduler{new Scheduler{}}, mTasks{}, mDisabledTasks{}, mMainTask{}, mGarbageCollectionTask{}
{
  mScheduler->init();
  // Setup main task
  mMainTask.set(mainInterval, TASK_FOREVER, mainTask);
  mScheduler->addTask(mMainTask);
  mMainTask.enable();
  // Setup garbage collection
  mGarbageCollectionTask.set(GARBAGE_COLLECTION_INTERVAL, TASK_FOREVER,
                             [this]() { RemoveDisabledTasks(); });
  mScheduler->addTask(mGarbageCollectionTask);
  mGarbageCollectionTask.enable();
}

CooperativeScheduler::~CooperativeScheduler()
{
  AbortAllTasks();
  RemoveDisabledTasks();  // Trigger the garbage collection one last time
}

TaskId CooperativeScheduler::AddOneShotTask(const TaskDuration delay, const TaskCallback& task,
                                            const TaskDuration timeout)
{
  return AddTask(TaskType::ONE_SHOT, delay, timeout, task);
}

TaskId CooperativeScheduler::AddCyclicTask(const TaskDuration interval, const TaskCallback& task,
                                           const TaskDuration timeout)
{
  return AddTask(TaskType::CYCLIC, interval, timeout, task);
}

CooperativeScheduler::ExecutionResult CooperativeScheduler::ExecuteNext()
{
  if (not mScheduler)
  {
    return ExecutionResult::ERR_INIT;
  }
  return (mScheduler->execute() ? ExecutionResult::IDLE : ExecutionResult::OK);
}

bool CooperativeScheduler::AbortTask(const TaskId id)
{
  auto item = mTasks.find(id);
  if (item == mTasks.end())
  {
    return false;
  }
  item->second->abort();  // This will lead to the execution of MarkTaskAsDisabled for this task.
  return true;
}

void CooperativeScheduler::AbortAllTasks() { mScheduler->disableAll(); }

TaskId CooperativeScheduler::AddTask(const TaskType type, const TaskDuration timespan,
                                     const TaskDuration timeout, const TaskCallback& task)
{
  std::unique_ptr<Task> t{new Task(timespan, TASK_ONCE, task, mScheduler.get(), true, nullptr,
                                   [this]() { MarkTaskAsDisabled(); })};
  const TaskId id = t->getId();
  const auto item = mTasks.emplace(id, std::move(t));
  if (not item.second)
  {
    return INVALID_TASKID;
  }
  return id;
}

void CooperativeScheduler::RestartMainTask()
{
  mMainTask.enable();
  mGarbageCollectionTask.enable();
}

void CooperativeScheduler::RestartAllTasks() { mScheduler->enableAll(); }

void CooperativeScheduler::MarkTaskAsDisabled()
{
  const auto id = mScheduler->getCurrentTask()->getId();
  mDisabledTasks.emplace_back(id);
  // The next time the garbage collection is executed, this task will be removed.
}

void CooperativeScheduler::RemoveDisabledTasks()
{
  for (const auto id : mDisabledTasks)
  {
    auto item = mTasks.find(id);
    if (item == mTasks.end())
    {
      continue;  // Task already removed.
    }
    auto* task = item->second.get();
    if (task)
    {
      mScheduler->deleteTask(*task);
    }
    mTasks.erase(item);
  }
  mDisabledTasks.clear();
}