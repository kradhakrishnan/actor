#include <list>
#include <memory>

#include "actor.h"
#include "iocore/unit-test.h"
#include "iocore/sysconf.h"
#include "iocore/logger.h"

using namespace std;
using namespace bblocks;

class ActorTest : public UnitTest {};

class Pinger;

struct PingPongEvent
{
	PingPongEvent(Pinger * pinger) : count_(0), pinger_(pinger) {}

	int count_;
	Pinger * pinger_;
};

atomic<int> exitCounter(SysConf::NumCores());

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
		INFO(log_) << this << " got event with count " << event.count_
				   << " from " << event.pinger_;

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

TEST_F(ActorTest, PingPongEvent)
{
	list<unique_ptr<Pinger> > pingers;
	for (size_t i = 0; i < SysConf::NumCores(); i++) {
		pingers.push_back(unique_ptr<Pinger>(new Pinger("pinger_" + i)));
	}

	unique_ptr<Pinger> & main = *pingers.begin();
	for (auto & p : pingers) {
		p->Send(PingPongEvent(main.get()));
	}

	while (exitCounter);
}

int
main(int argc, char ** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
