#include "psf/Node.h"
#include "psf/common.h"
#include <atomic>
#include <memory>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

namespace psf
{

Node::Node(std::string name)
	: name(std::move(name)), ipAddress(asio::ip::make_address_v4(getListenIpFromEnvVar().c_str())),
	  ioContext(std::make_shared<IOContext>()), registrySocket(psf::getRegistrySocketFromEnvVar())
{
	ioThread = std::thread([&ioContext = ioContext]() {
		// prevents ioContext from exiting.
		auto work = asio::require(ioContext->get_executor(), asio::execution::outstanding_work_t::tracked);
		ioContext->run();
	});

	registryThread = std::thread([&isStopped = isStopped, &registrySocket = registrySocket,
								  &endpointContainersForPublishers = endpointContainersForPublishers]() {
		registry::RegistryClient registryClient(registrySocket);
		std::vector<Endpoint> newEndpoints;

		while (!isStopped.load(std::memory_order_relaxed))
		{
			for (auto &c : endpointContainersForPublishers)
			{
				newEndpoints.clear();

				// read endpoints from registry
				auto endpointsFromRegistry = registryClient.getSubscriberEndpointsForTopic(c->topicName);
				for (auto const ep : endpointsFromRegistry)
				{
					newEndpoints.emplace_back(Endpoint(asio::ip::make_address_v4(ep.getAddress()), ep.getPort()));
				}

				// set new endpoints
				if (!(newEndpoints == c->endpoints))
				{
					std::lock_guard<std::mutex> lock(c->mtx);
					c->endpoints = newEndpoints;
				}
			}

			std::this_thread::sleep_for(100ms);
		}
	});
};

void Node::setSpinnerFunc(SpinnerFunc &&s)
{
	spinnerFunc = std::forward<SpinnerFunc>(s);
}

void Node::spin()
{
	assert(spinnerFunc);
	while (!isStopped.load(std::memory_order_relaxed))
	{
		spinnerFunc();
	};
};

void Node::stop()
{
	isStopped = true;
	ioContext->stop();
};

void Node::waitData()
{
	notifyOnDataFlag.wait(false);
	notifyOnDataFlag.clear(std::memory_order_release);
}

Node::~Node()
{
	ioThread.join();
	registryThread.join();
}

} // namespace psf
