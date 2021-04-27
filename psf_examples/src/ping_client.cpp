#include "msgs/PsfExamples.capnp.h"
#include "psf/Node.h"
#include <thread>

using namespace psf;
using namespace std::chrono_literals;

int main()
{
	try
	{
		Node n("ping_client");
		auto sub = n.createSubscriber<Ping>("/pong");
		auto pub = n.createPublisher<Ping>("/ping");

		::capnp::MallocMessageBuilder msgBuilder;
		auto pingMsg = msgBuilder.initRoot<Ping>();

		n.setSpinnerFunc([&]() {
			{
				if (pub->subscribersNum() > 0)
				{
					pingMsg.setTimestampNs(std::chrono::steady_clock::now().time_since_epoch().count());
					pub->publish(pingMsg);

					n.waitData();

					// print latency
					auto receivedTimestampNs = sub->getMsgReader().getTimestampNs();
					auto nowNs = std::chrono::steady_clock::now().time_since_epoch().count();
					std::cout << "RTT is " << static_cast<double>(nowNs - receivedTimestampNs) / 1000'000.0 << "ms"
							  << std::endl;
				}

				std::this_thread::sleep_for(1s);
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
