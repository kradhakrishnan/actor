#pragma once

#include <list>
#include <atomic>

#include "logger.h"
#include "util.h"
#include "schd/schd-helper.h"

namespace bblocks {

using namespace std;

//...................................................................................... Thread ....

class Thread
{
public:

	friend class NonBlockingThreadPool;
	friend class ThreadCtx;

	Thread(const string & logPath)
		: log_(logPath)
		, tid_(-1)
		, ctx_pool_(NULL)
	{}

	virtual ~Thread();

	void StartBlockingThread()
	{
		int ok = pthread_create(&tid_, /*attr=*/ NULL, ThFn, (void *)this);
		INVARIANT(!ok);

		INFO(log_) << "Thread " << tid_ << " created. (instance " << (long) this << ")";
	}

	void StartNonBlockingThread()
	{
		int ok = pthread_create(&tid_, /*attr=*/ NULL, ThFn, (void *)this);
		INVARIANT(!ok);

		INFO(log_) << "Thread " << tid_ << " created.";
	}

	void EnableThreadCancellation()
	{
		int status = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, /*oldstate=*/ NULL);
		(void) status;
		ASSERT(!status);
	}

	void DisableThreadCancellation()
	{
		int status = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, /*oldstate=*/ NULL);
		(void) status;
		ASSERT(!status);
	}

	void Cancel()
	{
		int status = pthread_cancel(tid_);
		INVARIANT(!status);
	}

	void Detach()
	{
		int status = pthread_detach(tid_);
		INVARIANT(!status);
	}

	virtual void Stop()
	{
		void * ret;
		int status = pthread_join(tid_, &ret);
		INVARIANT(!status || ret == PTHREAD_CANCELED);
	}

	void SetProcessorAffinity()
	{
		const uint32_t core = RRCpuId::Instance().GetId();

		INFO(log_) << "Binding to core " << core;

		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(core, &cpuset);

		int status = pthread_setaffinity_np(tid_, sizeof(cpuset), &cpuset);
		INVARIANT(!status);
	}

	void Destroy();

	static void * ThFn(void * args);

protected:

	virtual bool ShouldYield()
	{
		/*
		 * This is a query used by async process to make sure they are not blocking other
		 * operations during long CPU intensive work.
		 */
		 return false;
	}

	virtual void * ThreadMain() = 0;

	typedef list<uint8_t *> pool_t;

	string log_;
	pthread_t tid_;
	pool_t * ctx_pool_;
};

}
