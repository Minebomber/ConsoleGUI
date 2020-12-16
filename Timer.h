#pragma once
#include <functional>
#include <chrono>
#include <thread>
#include <future>

namespace gui {
class Timer {
private:
	std::chrono::milliseconds mInterval;
	std::function<void()> mAction;
	std::future<void> mFuture;
	bool mRunning = false;

	void Runloop() {
		while (mRunning) {
			if (mAction) mAction();
			std::this_thread::sleep_for(mInterval);
		}
	}
public:
	Timer(std::chrono::milliseconds i) : mInterval(i) {}
	Timer(std::chrono::milliseconds i, std::function<void()> a) : mInterval(i), mAction(a) {}

	const std::function<void()>& GetAction() const { return mAction; }
	void SetAction(std::function<void()> a) { mAction = a; }

	const std::chrono::milliseconds& GetInterval() const { return mInterval; }
	void SetInterval(std::chrono::milliseconds i) { mInterval = i; }

	void Start() {
		if (mRunning) return;
		mRunning = true;
		mFuture = std::async(std::launch::async, std::bind(&Timer::Runloop, this));
	}

	void Stop() {
		mRunning = false;
	}
	
};

}