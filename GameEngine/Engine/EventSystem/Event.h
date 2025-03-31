#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include <memory>

template <typename... Args>
class Event {
public:
	class Connection; // Forward declaration

	using Handler = std::function<void(Args...)>;  // General Handler type with Args

	virtual ~Event() = default;

	// Fire method to trigger the event and pass arguments to listeners
	void Fire(Args... args) {
		// Clean up disconnected handlers (those that are nullptr)
		handlers.erase(
			std::remove_if(handlers.begin(), handlers.end(),
						   [](const auto& handler) { return !handler; }),
			handlers.end());

		// Call the handler with the arguments
		for (auto& handler : handlers) {
			if (handler) {
				(*handler)(std::forward<Args>(args)...);
			}
		}
	}

	// Method to add event listeners and return a Connection for disconnection
	Connection OnEvent(Handler handler) {
		auto handlerPtr = std::make_shared<Handler>(std::move(handler));
		handlers.push_back(handlerPtr);
		return Connection(handlerPtr);  // Return the connection to manage the handler
	}

	// Nested Connection class for managing handler disconnection
	class Connection {
	private:
		std::shared_ptr<Handler> handlerPtr;

	public:
		Connection(std::shared_ptr<Handler> handler) : handlerPtr(std::move(handler)) {}

		// Disconnect the handler
		void Disconnect() {
			handlerPtr.reset();
		}
	};

protected:
	// Vector to hold event handlers as shared_ptrs
	std::vector<std::shared_ptr<Handler>> handlers;
};
