#include "msgs/PsfPerfTests.capnp.h"
#include "psf/Node.h"
#include "statistics_tracker.hpp"
#include <capnp/message.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using namespace psf;

int main()
{
	try
	{
		Node n("steady_epoch_publisher");
		auto pub = n.createPublisher<SteadyEpoch>("/steady_epoch");

		::capnp::MallocMessageBuilder msgBuilder;
		auto steadyEpoch = msgBuilder.initRoot<SteadyEpoch>();

		auto lastRun = std::chrono::steady_clock::now();
		n.setSpinnerFunc([&]() {
			{
				steadyEpoch.setValue(std::chrono::steady_clock::now().time_since_epoch().count());
				pub->publish(steadyEpoch);

				auto nextRun = lastRun + 1ms;
				lastRun = nextRun;
				std::this_thread::sleep_until(nextRun);
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
