#pragma once

#include <condition_variable>
#include <mutex>

// простая реализация счетного семафора
class semaphore {
  public:
    explicit inline semaphore(size_t count) : count(count) {}

    void acquire() {
	std::unique_lock<std::mutex> lock(mtx);
	c.wait(lock, [&] { return count != 0; });
	count--;
    }
    void release() {
	std::lock_guard<std::mutex> lock(mtx);
	count++;
	c.notify_one();
    }

  private:
    mutable std::mutex mtx;
    size_t count;
    std::condition_variable c;
};