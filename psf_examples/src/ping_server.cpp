#include "msgs/PsfExamples.capnp.h"
#include "psf/Node.h"

using namespace psf;
using namespace std::chrono_literals;

int main()
{
	try
	{
		Node n("ping_server");
		auto sub = n.createSubscriber<Ping>("/ping");
		auto pub = n.createPublisher<Ping>("/pong");

		::capnp::MallocMessageBuilder msgBuilder;
		auto pingMsg = msgBuilder.initRoot<Ping>();

		n.setSpinnerFunc([&]() {
			{
				n.waitData();
				pingMsg.setTimestampNs(sub->getMsgReader().getTimestampNs());
				pub->publish(pingMsg);
			}
		});
		n.spin();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
