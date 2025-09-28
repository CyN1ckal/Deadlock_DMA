#pragma once

#include <chrono>
#include <iomanip>
#include <map>
#include <vector>
#include <cstring>
#include <memory>
#include <ctime>
#include <utility>
#include <string>

#include "ctpl_stl.h"
#include "croncpp.h"

namespace Cppsched {

    using Clock = std::chrono::system_clock;

    class BadDateFormat : public std::exception {
    public:
        explicit BadDateFormat(std::string msg) : msg_(std::move(msg)) {}

        const char *what() const noexcept override { return (msg_.c_str()); }

    private:
        std::string msg_;
    };

    class TaskAlreadyExists : public std::exception {
    public:
        explicit TaskAlreadyExists(std::string msg) : msg_(std::move(msg)) {}

        const char *what() const noexcept override { return (msg_.c_str()); }

    private:
        std::string msg_;
    };

    class BadCronExpression : public std::exception {
    public:
        explicit BadCronExpression(std::string msg) : msg_(std::move(msg)) {}

        const char *what() const noexcept override { return (msg_.c_str()); }

    private:
        std::string msg_;
    };

    struct TaskReport {
      TaskReport( std::string id, std::string time_str, std::string next_run_str, bool enabled): id(id), time_str(time_str), next_run_str(next_run_str), enabled(enabled)
      {}

      std::string id;
      std::string time_str;
      std::string next_run_str;
      bool enabled;
    };

    class Task {
    public:
        explicit Task(const std::string &task_id, const std::string &time_str, std::function<void()> &&f, bool recur = false, bool interval = false, bool enabled  = true) :
                id(task_id), time_str(time_str), f(std::move(f)), recur(recur), interval(interval), enabled(enabled) {}

        virtual Clock::time_point get_new_time() const = 0;

        std::string id;         // Unique ID or user-defined name for the task
        std::string time_str;   // String represention of the time trigger
        std::function<void()> f;

        bool recur;
        bool interval;
        bool enabled;           // Flag to indicate if the task is enabled
        bool removed {false};   // Flag to indicate if the task is removed. Helps dealing with interval removal.
    };

    class InTask : public Task {
    public:
        explicit InTask(const std::string &task_id, const std::string &time_str, std::function<void()> &&f) : Task(task_id, time_str, std::move(f)) {}

        // dummy time_point because it's not used
        Clock::time_point get_new_time() const override { return Clock::time_point(Clock::duration(0)); }
    };

    class AtTask : public Task { // Basically same as in
    public:
        explicit AtTask(const std::string &task_id, const std::string &time_str, std::function<void()> &&f) : Task(task_id, time_str, std::move(f)) {}

        // dummy time_point because it's not used
        Clock::time_point get_new_time() const override { return Clock::time_point(Clock::duration(0)); }
    };

    class EveryTask : public Task {
    public:
        EveryTask(const std::string &task_id, const std::string &time_str, Clock::duration time, std::function<void()> &&f, bool interval = false) :
                Task(task_id, time_str, std::move(f), true, interval), time(time) {}

        Clock::time_point get_new_time() const override {
          return Clock::now() + time;
        };
        Clock::duration time;
    };


    class CronTask : public Task {
    public:
        CronTask(const std::string &task_id, const std::string &time_str, std::string expression, std::function<void()> &&f) : Task(task_id, time_str, std::move(f), true),
                                                                       exp(std::move(expression)) {}

        Clock::time_point get_new_time() const override {
            Clock::time_point next;
            try
            {
                auto cron = cron::make_cron(exp);

                next = cron::cron_next(cron, Clock::now());
            }
            catch (cron::bad_cronexpr const & e)
            {
                throw BadCronExpression(std::string(e.what()));
            }

            return next;
        };

        std::string exp;
    };

    inline bool try_parse(std::tm &tm, const std::string &expression, const std::string &format) {
      std::stringstream ss(expression);
      return !(ss >> std::get_time(&tm, format.c_str())).fail();
    }

    class InterruptableSleep {
        using Clock = std::chrono::system_clock;

        // InterruptableSleep offers a sleep that can be interrupted by any thread.
        // It can be interrupted multiple times
        // and be interrupted before any sleep is called (the sleep will immediately complete)
        // Has same interface as condition_variables and futures, except with sleep instead of wait.
        // For a given object, sleep can be called on multiple threads safely, but is not recommended as behaviour is undefined.

    public:
        InterruptableSleep() : interrupted(false) {
        }

        InterruptableSleep(const InterruptableSleep &) = delete;

        InterruptableSleep(InterruptableSleep &&) noexcept = delete;

        ~InterruptableSleep() noexcept = default;

        InterruptableSleep &operator=(const InterruptableSleep &) noexcept = delete;

        InterruptableSleep &operator=(InterruptableSleep &&) noexcept = delete;

        void sleep_for(Clock::duration duration) {
          std::unique_lock<std::mutex> ul(m);
          cv.wait_for(ul, duration, [this] { return interrupted; });
          interrupted = false;
        }

        void sleep_until(Clock::time_point time) {
          std::unique_lock<std::mutex> ul(m);
          cv.wait_until(ul, time, [this] { return interrupted; });
          interrupted = false;
        }

        void sleep() {
          std::unique_lock<std::mutex> ul(m);
          cv.wait(ul, [this] { return interrupted; });
          interrupted = false;
        }

        void interrupt() {
          std::lock_guard<std::mutex> lg(m);
          interrupted = true;
          cv.notify_one();
        }

    private:
        bool interrupted;
        std::mutex m;
        std::condition_variable cv;
    };

    class Scheduler {
    public:
        explicit Scheduler(unsigned int max_n_tasks = 4) : done(false), threads(max_n_tasks + 1) {
          threads.push([this](int) {
              while (!done) {
                if (tasks.empty()) {
                  sleeper.sleep();
                } else {
                  auto time_of_first_task = (*tasks.begin()).first;
                  sleeper.sleep_until(time_of_first_task);
                }
                manage_tasks();
              }
          });
        }

        Scheduler(const Scheduler &) = delete;

        Scheduler(Scheduler &&) noexcept = delete;

        Scheduler &operator=(const Scheduler &) = delete;

        Scheduler &operator=(Scheduler &&) noexcept = delete;

        ~Scheduler() {
          done = true;
          sleeper.interrupt();
        }

        template<typename _Callable, typename... _Args>
        void in(const std::string &task_id, const Clock::duration time, _Callable &&f, _Args &&... args) {
          std::string time_str {"in: " + format_duration(time)};
          std::shared_ptr<Task> t = std::make_shared<InTask>(task_id, time_str,
                  std::bind(std::forward<_Callable>(f), std::forward<_Args>(args)...));
          add_task(task_id, Clock::now() + time, std::move(t));
        }

        template<typename _Callable, typename... _Args>
        void at(const std::string &task_id, const Clock::time_point time, _Callable &&f, _Args &&... args) {
          std::string time_str {"at: " + format_time_point("%F %T %z", time)};
          std::shared_ptr<Task> t = std::make_shared<AtTask>(task_id, time_str,
                  std::bind(std::forward<_Callable>(f), std::forward<_Args>(args)...));
          add_task(task_id, time, std::move(t));
        }

        template<typename _Callable, typename... _Args>
        void at(const std::string &task_id, const std::string &time, _Callable &&f, _Args &&... args) {
          // get current time as a tm object
          auto time_now = Clock::to_time_t(Clock::now());
          std::tm tm = *std::localtime(&time_now);

          // our final time as a time_point
          Clock::time_point tp;

          if (try_parse(tm, time, "%H:%M:%S")) {
            // convert tm back to time_t, then to a time_point and assign to final
            tp = Clock::from_time_t(std::mktime(&tm));

            // if we've already passed this time, the user will mean next day, so add a day.
            if (Clock::now() >= tp)
              tp += std::chrono::hours(24);
          } else if (try_parse(tm, time, "%Y-%m-%d %H:%M:%S")) {
            tp = Clock::from_time_t(std::mktime(&tm));
          } else if (try_parse(tm, time, "%Y/%m/%d %H:%M:%S")) {
            tp = Clock::from_time_t(std::mktime(&tm));
          } else {
            // could not parse time
            throw BadDateFormat("Cannot parse time string: " + time);
          }

          // std::string time_str {"at: " + time};
          std::string time_str {"at: " + format_time_point("%F %T %z", tp)};
          std::shared_ptr<Task> t = std::make_shared<AtTask>(task_id, time_str,
                  std::bind(std::forward<_Callable>(f), std::forward<_Args>(args)...));
          add_task(task_id, tp, std::move(t));
        }

        template<typename _Callable, typename... _Args>
        void every(const std::string &task_id, const Clock::duration time, _Callable &&f, _Args &&... args) {
          std::string time_str {"every: " + format_duration(time)};
          std::shared_ptr<Task> t = std::make_shared<EveryTask>(task_id, time_str, time, std::bind(std::forward<_Callable>(f),
                                                                                std::forward<_Args>(args)...));
          auto next_time = t->get_new_time();
          add_task(task_id, next_time, std::move(t));
        }

        template<typename _Callable, typename... _Args>
        void cron(const std::string &task_id, const std::string &expression, _Callable &&f, _Args &&... args) {
          std::string time_str {"cron: " + expression};
          std::shared_ptr<Task> t = std::make_shared<CronTask>(task_id, time_str, expression, std::bind(std::forward<_Callable>(f),
                                                                                      std::forward<_Args>(args)...));
          auto next_time = t->get_new_time();
          add_task(task_id, next_time, std::move(t));
        }

        template<typename _Callable, typename... _Args>
        void interval(const std::string &task_id, const Clock::duration time, _Callable &&f, _Args &&... args) {
          std::string time_str {"interval: " + format_duration(time)};
          std::shared_ptr<Task> t = std::make_shared<EveryTask>(task_id, time_str, time, std::bind(std::forward<_Callable>(f),
                                                                                std::forward<_Args>(args)...), true);
          auto next_time = t->get_new_time();
          add_task(task_id, Clock::now(), std::move(t));
        }

        // Method to remove a task by ID or name
        bool remove_task(const std::string& task_id)
        {
          std::lock_guard<std::mutex> l(lock);

          // Find the task in the tasks_map
          auto task_iterator = tasks_map.find(task_id);
          if (task_iterator != tasks_map.end()) {
            task_iterator->second->second->removed = true;;
            tasks_map.erase(task_iterator);

            return true;
          }

          return false;
        }

        // Method to disable a task by ID or name
        bool disable_task(const std::string& task_id)
        {
          std::lock_guard<std::mutex> l(lock);

          // Find the task in the tasks_map
          auto task_map_iterator = tasks_map.find(task_id);
          if (task_map_iterator != tasks_map.end()) {
            // Disable the task
            auto &task_pair {task_map_iterator->second};
            auto &task {task_pair->second};
            task->enabled = false;

            return true;
          }

          return false;
        }

        // Method to enable a task by ID or name
        bool enable_task(const std::string& task_id)
        {
          std::lock_guard<std::mutex> l(lock);

          // Find the task in the tasks_map
          auto task_map_iterator = tasks_map.find(task_id);
          if (task_map_iterator != tasks_map.end()) {
            // Enable the task
            auto &task_pair {task_map_iterator->second};
            auto &task {task_pair->second};
            task->enabled = true;

            return true;
          }

          return false;
        }

        std::vector<TaskReport> get_tasks_list()
        {
          std::vector<TaskReport> v;

          {
            std::lock_guard<std::mutex> l(lock);
            for (auto &map_pair : tasks_map)
            {
              auto &task_pair {map_pair.second};
              auto &task {task_pair->second};
              auto next_run {task->get_new_time()};
              // We'll have next_run precission of just 1 sec for now, we might
              // increase this in the future if needed. We just need to figure
              // out how.
              v.push_back(TaskReport(task->id, task->time_str, format_time_point("%F %T %z", next_run), task->enabled));
            }
          }

          return v;
        }

    private:
        std::atomic<bool> done;

        InterruptableSleep sleeper;

        std::multimap<Clock::time_point, std::shared_ptr<Task>> tasks;
        std::multimap<Clock::time_point, std::shared_ptr<Task>> completed_interval_tasks;
        std::map<std::string, std::multimap<Clock::time_point, std::shared_ptr<Task>>::iterator> tasks_map;
        std::mutex lock;
        ctpl::thread_pool threads;

        void add_task(const Clock::time_point time, std::shared_ptr<Task> t) {
          std::lock_guard<std::mutex> l(lock);
          const std::string &task_id {t->id};
          auto inserted_task = tasks.emplace(time, std::move(t));
          tasks_map[task_id] = inserted_task; // Map task ID to its iterator in tasks multimap
          sleeper.interrupt();
        }

        void add_task(const std::string& task_id, const Clock::time_point time, std::shared_ptr<Task> t) {
          std::lock_guard<std::mutex> l(lock);
          if (tasks_map.find(task_id) == tasks_map.end()) {
            auto inserted_task = tasks.emplace(time, std::move(t));
            tasks_map[task_id] = inserted_task; // Map task ID to its iterator in tasks multimap
            sleeper.interrupt();
          } else {
            throw TaskAlreadyExists("Task with id <" + task_id + "> already exists");
          }
        }

        void manage_tasks() {
          std::lock_guard<std::mutex> l(lock);

          auto end_of_tasks_to_run = tasks.upper_bound(Clock::now());

          // if there are any tasks to be run and removed
          if (end_of_tasks_to_run != tasks.begin()) {
            // keep track of tasks that will be re-added
            decltype(tasks) recurred_tasks;
            // keep track of tasks that will be removed
            std::vector<std::shared_ptr<Task>> non_recurred_tasks;

            // for all tasks that have been triggered
            for (auto i = tasks.begin(); i != end_of_tasks_to_run; ++i) {
              auto &task = (*i).second;
              if (task->interval) {
                // if it's an interval task, only add the task back after f() is completed
                if (task->enabled && ! task->removed) {
                  // Temporarily save task until completed
                  auto inserted_task = completed_interval_tasks.insert(*i);
                  tasks_map[task->id] = inserted_task;

                  // Run
                  threads.push([this, task, inserted_task](int) {
                      task->f();
                      // no risk of race-condition,
                      // add_task() will wait for manage_tasks() to release lock
                      add_task(task->get_new_time(), task);
                      completed_interval_tasks.erase(inserted_task);
                      });
                }
                else
                {
                  recurred_tasks.emplace(task->get_new_time(), std::move(task));
                }
              } else {
                if (task->enabled && ! task->removed) {
                  threads.push([task](int) {
                      task->f();
                      });
                }

                if (task->recur) {
                  // calculate time of next run and add the new task to the tasks to be recurred
                  recurred_tasks.emplace(task->get_new_time(), std::move(task));
                } else {
                  // save non recurred to remove from tasks_map
                  non_recurred_tasks.push_back(task);
                }
              }
            }

            // remove the completed tasks
            tasks.erase(tasks.begin(), end_of_tasks_to_run);

            // re-add the tasks that are recurring
            for (auto &task_pair : recurred_tasks)
            {
              if (! task_pair.second->removed)
              {
                // tasks.emplace(task_pair.first, std::move(task_pair.second));
                auto &time {task_pair.first};
                auto &task {task_pair.second};
                const std::string &task_id {task->id};
                auto inserted_task = tasks.emplace(time, std::move(task));
                tasks_map[task_id] = inserted_task; // Map task ID to its iterator in tasks multimap
              }
            }

            // remove from tasks_map
            for (auto &task : non_recurred_tasks) {
              auto task_map_iterator = tasks_map.find(task->id);
              if (task_map_iterator != tasks_map.end()) {
                tasks_map.erase(task_map_iterator);
              }
            }
          }
        }

        inline std::string format_time_point(const std::string &format, const Clock::time_point date) const
        {
          char       buffer[80] = "";
          std::time_t date_c = std::chrono::system_clock::to_time_t(date);
          std::tm *date_tm = std::localtime(&date_c);

          if (strftime(buffer, sizeof(buffer), format.c_str(), date_tm) == 0)
          {
            throw BadDateFormat("Error in given format <" + format + ">");
          }

          return std::string(buffer);
        }

        inline std::string format_duration(std::chrono::nanoseconds timeunit) const
        {
          std::chrono::nanoseconds ns =
            std::chrono::duration_cast<std::chrono::nanoseconds>(timeunit);
          std::ostringstream os;
          bool               foundNonZero = false;
          os.fill('0');
          typedef std::chrono::duration<int, std::ratio<86400 * 365>> years;
          const auto y = std::chrono::duration_cast<years>(ns);
          if (y.count())
          {
            foundNonZero = true;
            os << y.count() << "y";
            ns -= y;
          }
          typedef std::chrono::duration<int, std::ratio<86400>> days;
          const auto d = std::chrono::duration_cast<days>(ns);
          if (d.count())
          {
            if (foundNonZero)
              os << ":";
            foundNonZero = true;
            os << d.count() << "d";
            ns -= d;
          }
          const auto h = std::chrono::duration_cast<std::chrono::hours>(ns);
          if (h.count())
          {
            if (foundNonZero)
              os << ":";
            foundNonZero = true;
            os << h.count() << "h";
            ns -= h;
          }
          const auto m = std::chrono::duration_cast<std::chrono::minutes>(ns);
          if (m.count())
          {
            if (foundNonZero)
              os << ":";
            foundNonZero = true;
            os << m.count() << "m";
            ns -= m;
          }
          const auto s = std::chrono::duration_cast<std::chrono::seconds>(ns);
          if (s.count())
          {
            if (foundNonZero)
              os << ":";
            foundNonZero = true;
            os << s.count() << "s";
            ns -= s;
          }
          const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(ns);
          if (ms.count())
          {
            if (foundNonZero)
              os << ":";
            foundNonZero = true;
            os << ms.count() << "ms";
            ns -= ms;
          }
          const auto us = std::chrono::duration_cast<std::chrono::microseconds>(ns);
          if (us.count())
          {
            if (foundNonZero)
              os << ":";
            os << us.count() << "us";
            ns -= us;
          }
          if (ns.count())
          {
            if (foundNonZero)
              os << ":";
            foundNonZero = true;
            os << ns.count() << "ns";
          }
          if (! foundNonZero)
          {
            os << "0s";
          }
          return os.str();
        }
    };

} // namespace Cppsched
