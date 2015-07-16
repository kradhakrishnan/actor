#include "schd/thread.h"
#include "schd/thread-ctx.h"

using namespace bblocks;

Thread::~Thread()
{
	Destroy();
}

void
Thread::Destroy()
{
	if (ctx_pool_) {

		INFO(log_) << "Cleaning up thread context for " << (uint64_t) this;

		/*
		 * There is blanket assumption here that the thread is no longer running.
		 * TODO: Add verification for the invariant
		 */
		ThreadCtx::Cleanup(ctx_pool_);
		ctx_pool_ = NULL;
	}

	INFO(log_) << "Thread " << tid_ << " destroyed.";
}

void *
Thread::ThFn(void * args)
{
	INVARIANT(args);

	Thread * th = (Thread *) args;

	ThreadCtx::Init(th);

	void * thstatus = th->ThreadMain();

	ThreadCtx::Cleanup();

	return thstatus;
}


