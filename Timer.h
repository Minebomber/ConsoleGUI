#pragma once
#include <functional>
#include <chrono>
#include <thread>
#include <future>

namespace gui {
class Timer {
private:
	std::future<void> mFuture;
	bool mRunning = false;

	void Runloop() {
		while (mRunning) {
			if (action) action();
			std::this_thread::sleep_for(interval);
		}
	}
public:
	std::chrono::milliseconds interval;
	std::function<void()> action;

	Timer(std::chrono::milliseconds i) : interval(i) {}
	Timer(std::chrono::milliseconds i, std::function<void()> a) : interval(i), action(a) {}

	void Start() {
		if (mRunning) return;
		mRunning = true;
		mFuture = std::async(std::launch::async, std::bind(&Timer::Runloop, this));
	}

	void Stop() { mRunning = false; }
	
};

}