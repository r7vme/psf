#include "psf/registry/RegistryServer.h"

namespace psf::registry
{

kj::Promise<void> RegistryServer::registerEndpoint(RegisterEndpointContext context)
{
	auto input = context.getParams().getEndpoint();

	Endpoint ep;
	ep.type = static_cast<Endpoint::Type>(input.getType());
	ep.address = input.getAddress();
	ep.port = input.getPort();
	ep.nodeName = input.getNodeName();
	ep.topicName = input.getTopicName();
	registryImpl.registerEndpoint(ep);

	context.getResults().setResult(true);
	return kj::READY_NOW;
}

kj::Promise<void> RegistryServer::getSubscriberEndpointsForTopic(GetSubscriberEndpointsForTopicContext context)
{
	auto subscribers = registryImpl.getSubscribersForTopic(context.getParams().getTopicName());

	auto returnList = context.getResults().initSubscribers(static_cast<uint32>(subscribers.size()));
	for (uint32 i = 0; i < static_cast<uint32>(subscribers.size()); i++)
	{
		auto ep = returnList[i];
		ep.setType(static_cast<rpc::Endpoint::Type>(subscribers[i].type));
		ep.setAddress(subscribers[i].address);
		ep.setPort(subscribers[i].port);
		ep.setNodeName(subscribers[i].nodeName);
		ep.setTopicName(subscribers[i].topicName);
	}
	return kj::READY_NOW;
}

} // namespace psf::registry
