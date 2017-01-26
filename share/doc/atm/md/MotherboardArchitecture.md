# ATM motherboard architecture

The advanced tessellated machine consists of four by four modules, which are called "nodes".
These nodes are connected to each other via eight connections, which means that every node is
connected to the one above, below, to the left, to the right and to the corners. All nodes can
send 16-bit integers through those connections, which lets them corporate and it's the job of the
programmer to organize their actions to get a final result.

There are different types of nodes: Processors, graphic adapters, timers, and keyboards are currently
available and others may follow sooner or later. The nodes are organized in a standardized way to
maintain the exchangeability of created programs, which looks like this:

	-----------------
	|   |   |   |   |
	-----------------
	| G | P | P |   |
	-----------------
	|   | P | P | K |
	-----------------
	|   | T |   |   |
	-----------------
	
Each character stands for one type of nodes: The most important is `P`, which stands for
[programmable processors](ProcessorNode.md). All other types aren't programmable but beeing
controlled by the processors. The next nodes are: `G`, which is the [graphics adapter](GraphicsAdapter.md),
`T`, the [time adapter](TimeAdapter.md) and `K`, the [keyboard adapter](KeyboardAdapter.md).
There is a lot of empty space in this diagram, since new nodes may follow which need a place on the board too.
