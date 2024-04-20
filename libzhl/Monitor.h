#pragma once

#include <atomic>
#include <mutex>
#include <optional>
#include <queue>

namespace ZHL {
	/* Thread-safe monitor. Stores values of type T that can be pushed and retrieved
	 * concurrently by multiple threads in a safe way.
	 *
	 * T must meet the requirements to be storable in a std::queue.
	 */
	template<typename T>
	class Monitor {
	public:
		T Get() {
			std::unique_lock<std::mutex> lck(_m);
			while (_elements.empty()) {
				_cv.wait(lck);
			}
			T result = _elements.front();
			_elements.pop();
			return result;
		}

		std::optional<T> GetMaybe() {
			std::unique_lock<std::mutex> lck(_m);
			if (_elements.empty()) {
				return std::nullopt;
			}
			T result = _elements.front();
			_elements.pop();
			return result;
		}

		void PushNoSync(T const& t) {
			_elements.push(t);
		}

		void PushNoSync(T&& t) {
			_elements.push(std::move(t));
		}

		void Push(T const& t) {
			std::unique_lock<std::mutex> lck(_m);
			_elements.push(t);
			_cv.notify_all();
		}

		void Push(T&& t) {
			std::unique_lock<std::mutex> lck(_m);
			_elements.push(std::move(t));
			_cv.notify_all();
		}

	private:
		std::queue<T> _elements;
		std::mutex _m;
		std::condition_variable _cv;
	};
}