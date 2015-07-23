#pragma once

#include "iocore/thread.h"

namespace bblocks {

// .................................................... EventHandler<EVENT> ....

template<class EVENT>
class EventHandler
{
protected:

	virtual void Handle(const EVENT & event) = 0;
};

// ...................................................... EventActor<EVENT> ....

template<class EVENT>
class EventActor : public EventHandler<EVENT>
{
public:

	virtual void Send(const EVENT & event) = 0;
};

// ..................................................... ActorThread<EVENT> ....

template<class EVENT>
class ThreadedReactor : public EventActor<EVENT>, public Thread
{
public:

	ThreadedReactor(const std::string & name)
		: Thread(name), eventQ_(name)
	{
		Start();
	}

	virtual ~ThreadedReactor()
	{
		Cancel();
		Join();
	}

	void Send(const EVENT & event) override
	{
		eventQ_.Push(event);
	}

protected:

	void * ThreadMain() override
	{
		while (true) {
			EVENT event = eventQ_.Pop();
			this->Handle(event);
		}

		return nullptr;
	}

	Queue<EVENT> eventQ_;
};

// ......................................................... Reactor<EVENT> ....

template<class EVENT>
class Reactor : protected EventActor<EVENT>
{
public:

	void Send(const EVENT & event) override
	{
		Handle(event);
	}
};

} // namespace bblocks
