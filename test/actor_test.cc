#include "actor_test.h"

using namespace std;
using namespace bblocks;

TEST_F(ActorTest, PingPongReactorTest)
{
	using namespace ppReactor;

	list<unique_ptr<Pinger> > pingers;
	for (size_t i = 0; i < SysConf::NumCores(); i++) {
		pingers.push_back(unique_ptr<Pinger>(new Pinger("pinger_" + i)));
	}

	unique_ptr<Pinger> & main = *pingers.begin();
	for (auto & p : pingers) {
		p->Send(PingPongEvent(main.get()));
	}
}

TEST_F(ActorTest, PingPongThreadedReactorTest)
{
	using namespace ppThreadedReactor;

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
