#pragma once
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <optional>

// Base Event class
class Event {
public:
	using Handler = std::function<void()>;  // General Handler type

	virtual ~Event() = default;  // Virtual destructor for polymorphic behavior

	// Pure virtual methods to fire and add handlers for events
	void Fire();  // To fire the event
	class Connection OnEvent(Handler handler);  // To add handlers and return connection

	// Connection class to manage handler disconnection
	class Connection {
	private:
		size_t index;

	public:
		Connection(size_t idx) : index(idx) {}

		void Disconnect() {
			if (index < handlers.size()) {
				handlers[index] = std::nullopt;  // Disconnect the handler by setting it to nullopt
			}
		}
	};

protected:
	// Static vector to hold event handlers (specialized in derived classes)
	static std::vector<std::optional<Handler>> handlers;
};