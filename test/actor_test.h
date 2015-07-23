#pragma once

#include <list>
#include <memory>

#include "actor.h"
#include "iocore/unit-test.h"
#include "iocore/sysconf.h"
#include "iocore/logger.h"

namespace bblocks {

class ActorTest : public UnitTest {};

namespace ppReactor {

class Pinger;

struct PingPongEvent
{
	PingPongEvent(Pinger * pinger) : count_(0), pinger_(pinger) {}

	int count_;
	Pinger * pinger_;
};

class Pinger : public Reactor<PingPongEvent>
{
public:

	Pinger(const std::string & name)
		: log_(name)
	{}

	// Keep it small because we use the stack to call up
	static const int MAX_EVENTS = 10;

protected:

	void Handle(const PingPongEvent & event) override
	{
		if (event.count_ > MAX_EVENTS) {
			return;
		}
		
		PingPongEvent out(this);
		out.count_ = event.count_ + 1;
		event.pinger_->Send(out);
	}

	const std::string log_;
};

} // namespace ppReactor


namespace ppThreadedReactor {

class Pinger;

struct PingPongEvent
{
	PingPongEvent(Pinger * pinger) : count_(0), pinger_(pinger) {}

	int count_;
	Pinger * pinger_;
};

std::atomic<int> exitCounter(SysConf::NumCores());

class Pinger : public ThreadedReactor<PingPongEvent>
{
public:

	Pinger(const std::string & name)
		: ThreadedReactor<PingPongEvent>(name)
		, log_(name)
	{}

	static const int MAX_EVENTS = 1000;

protected:

	void Handle(const PingPongEvent & event) override
	{
		if (event.count_ > MAX_EVENTS) {
			--exitCounter;
			return;
		}
		
		PingPongEvent out(this);
		out.count_ = event.count_ + 1;
		event.pinger_->Send(out);
	}

	const std::string log_;
};

} // namespace ppThreadedReactor
} // namespace bblocks
