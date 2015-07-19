#pragma once

#include "logger.h"
#include "util.h"

namespace bblocks {

template<class EVENT>
class EventHandler
{
public:

	virtual void Handle(EVENT && event) = 0;
};

template<class EVENT>
class EventThread : private Thread
{
public:

	EventThread(const std::string & name, EventHandler<EVENT> * eventHandler)
		: Thread(name), log_(name), eventHandler_(eventHandler)
	{
		Start();
	}

	virtual ~EventThread()
	{
		Cancel();
		Destroy();

		INVARIANT(events_.IsEmpty());
	}

	void Send(const EVENT && event)
	{
		events_.Push(event);
	}

	virtual void * Main(void *)
	{
		while (true) {
			EVENT event = events_.Pop();
			(*eventHandler_)(event);
		}

		return nullptr;
	}

private:

	Logger log_;
	EventHandler<EVENT> const * eventHandler_;
	Queue<EVENT> events_;
};

}
