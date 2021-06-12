//#pragma once
//
//
//enum class ActorEventType
//{
//	None = 0,
//	ChangeState, TileCollision, ActorCollision
//};
//
//#define BIT(n) ( 1 << (n) )
//
//enum class ActorEventCatagory
//{
//	None          = 0,
//	GraphicsEvent = BIT(0),
//	AudioEvent    = BIT(1),
//	LogicEvent    = BIT(2),
//	PhysicsEvent  = BIT(3)
//};
//
//#define EVENT_CLASS_TYPE(type)  static  ActorEventType staticEventType() { return EventType::type; }\
//								virtual ActorEventType eventType() const override { return StaticEventType(); }\
//								virtual std::string getName() const override { return #type; }
//
//#define EVENT_CLASS_CATEGORY(category) virtual ActorEventCatagory GetCategory() const override { return category; }
//
//class ActorEvent
//{
//public:
//	virtual ActorEventType eventType() const = 0;
//	virtual std::string getName() const = 0;
//	virtual ActorEventCatagory GetCategory() const = 0;
//
//	bool isInCategory(ActorEventCatagory category)
//	{
//		return GetCategory() & category;
//	}
//
//	bool handled = false;
//};
//
//class ActorEventHandler
//{
//public:
//	ActorEventHandler(ActorEvent& event)
//		: event(event) {}
//
//
//	template<typename T, typename Fn>
//	bool handle(const Fn& func) {
//		if (event.eventType() == T::StaticEventType()) {
//			event.handled |= func(static_cast<T&>(event));
//			return true;
//		}
//		return false;
//	}
//private:
//	ActorEvent& event;
//};
//
//class EventQueue
//{
//public:
//	void pushEvent(const ActorEvent& event) {
//		events.push(event);
//	}
//
//	bool pollEvent(ActorEvent& event) {
//		if (events.empty) {
//			return false;
//		}
//		else {
//			event = events.front();
//			events.pop();
//			return true;
//		}
//	}
//private:
//	std::queue<ActorEvent> events;
//};
//
