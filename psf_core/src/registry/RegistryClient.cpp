#include "psf/registry/RegistryClient.h"

namespace psf::registry
{

void RegistryClient::registerSubscriber(std::string const &nodeName, std::string const &topicName, uint32 address,
										uint16 port)
{
	auto registry = rpcClient.getMain<rpc::Registry>();

	auto request = registry.registerEndpointRequest();

	auto endpoint = request.getEndpoint();
	endpoint.setType(rpc::Endpoint::Type::SUBSCRIBER);
	endpoint.setAddress(address);
	endpoint.setPort(port);
	endpoint.setTopicName(topicName);
	endpoint.setNodeName(nodeName);

	auto response = request.send().wait(rpcClient.getWaitScope());
	if (!response.getResult())
	{
		throw std::runtime_error("registerEndpointRequest failed.");
	}
}

RegistryClient::EndpointsReader RegistryClient::getSubscriberEndpointsForTopic(std::string const &topicName)
{
	auto registry = rpcClient.getMain<rpc::Registry>();
	auto req = registry.getSubscriberEndpointsForTopicRequest();
	req.setTopicName(topicName);
	auto resp = req.send().wait(rpcClient.getWaitScope());
	return resp.getSubscribers();
}

} // namespace psf::registry
