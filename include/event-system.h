#pragma once

#include "event-handler.h"

namespace bblocks {

class EventSystem
{
public:

	/**
	 * Add an event handler that should invoked when the event system
	 * bootsup
	 */
	static void AddStartupEventHandler(EventHandler<int> && handler);

	/**
	 * Run event loop. This call blocks the calling thread until shutdown is
	 * called.
	 */
	static void Run(const size_t nthreads);

	/**
	 * Shutdown event system
	 */
	static void Shutdown();
};

}
