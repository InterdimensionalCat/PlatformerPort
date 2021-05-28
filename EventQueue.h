#pragma once
#include <queue>



template<typename T>
class EventQueue
{
public:
	void pushEvent(const std::shared_ptr<T>& event) {
		events.push(event);
	}

	bool pollEvent(std::shared_ptr<T>& event) {
		if (events.empty) {
			return false;
		}
		else {
			event = events.front();
			events.pop();
			return true;
		}
	}
private:
	std::queue<std::shared_ptr<T>> events;
};

