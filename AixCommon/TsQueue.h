#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>


//low lever thread safe queue

template<typename T>
class TsQueue
{
public:
	TsQueue() = default;
	TsQueue(const TsQueue<T>&) = delete;
	virtual ~TsQueue() { clear(); }

public:
	const T& front()
	{
		std::scoped_lock lock(muxQueue);
		return deqQueue.front();
	}

	const T& back()
	{
		std::scoped_lock lock(muxQueue);
		return deqQueue.back();
	}

	T pop_front()
	{
		std::scoped_lock lock(muxQueue);
		auto t = std::move(deqQueue.front());
		deqQueue.pop_front();
		return t;
	}

	T pop_back()
	{
		std::scoped_lock lock(muxQueue);
		auto t = std::move(deqQueue.back());
		deqQueue.pop_back();
		return t;
	}

	void push_back(const T& item)
	{
		std::scoped_lock lock(muxQueue);
		deqQueue.emplace_back(std::move(item));

		std::unique_lock<std::mutex> ul(muxBlocking);
		cvBlocking.notify_one();
	}

	// Adds an item to front of Queue
	void push_front(const T& item)
	{
		std::scoped_lock lock(muxQueue);
		deqQueue.emplace_front(std::move(item));

		std::unique_lock<std::mutex> ul(muxBlocking);
		cvBlocking.notify_one();
	}

	// Returns true if Queue has no items
	bool empty()
	{
		std::scoped_lock lock(muxQueue);
		return deqQueue.empty();
	}

	// Returns number of items in Queue
	size_t count()
	{
		std::scoped_lock lock(muxQueue);
		return deqQueue.size();
	}

	// Clears Queue
	void clear()
	{
		std::scoped_lock lock(muxQueue);
		deqQueue.clear();
	}

	void wait()
	{
		while (empty())
		{
			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.wait(ul);
		}
	}

protected:
	std::mutex muxQueue;
	std::deque<T> deqQueue;
	std::condition_variable cvBlocking;
	std::mutex muxBlocking;
};