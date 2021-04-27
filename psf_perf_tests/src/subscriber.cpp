#include <chrono>
#include <iostream>
#include <thread>

#include "StatisticsPrinter.hpp"
#include "msgs/PsfPerfTests.capnp.h"
#include "psf/Node.h"

using namespace psf;
using namespace std::chrono_literals;

int main()
{
	try
	{
		Node n("steady_epoch_subscriber");
		auto sub = n.createSubscriber<SteadyEpoch>("/steady_epoch");

		StatisticsPrinter statsPrinter;
		auto statsPrinterThread = std::thread(&StatisticsPrinter::run, &statsPrinter);

		n.setSpinnerFunc([&]() {
			n.waitData();
			auto publisherSteadyEpoch = sub->getMsgReader();
			auto now = std::chrono::steady_clock::now().time_since_epoch().count();

			assert(now > publisherSteadyEpoch.getValue());
			double latency = static_cast<double>(now - publisherSteadyEpoch.getValue());
			statsPrinter.addSample(latency);
		});

		n.spin();
		statsPrinterThread.join();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
