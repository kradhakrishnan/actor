# actor

A generalized framework for building event driven asynchronous systems designed to take advantage of modern multi-core processor architecture. It heavily favors "Stagged Event Driven Architecture" (SEDA) event driver model for pipelined event processing. The system is ideal for building IO intensive, latency sensitive applications like database, filesystem etc that leverage high throughput devices like SSDs and support high end communication hardware like 10+ Gbps.
