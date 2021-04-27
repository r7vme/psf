#pragma once
#include <capnp/ez-rpc.h>
#include <memory>

#include "psf/registry/Registry.h"
#include "psf/registry/RegistryRpc.capnp.h"
#include "psf/common.h"

namespace psf
{
namespace registry
{
class RegistryClient
{
  public:
	using EndpointsReader = ::capnp::List<psf::registry::rpc::Endpoint>::Reader;

	RegistryClient(std::string const &socket) : rpcClient(socket)
	{
	}
	void registerSubscriber(std::string const &nodeName, std::string const &topicName, uint32 address, uint16 port);
	EndpointsReader getSubscriberEndpointsForTopic(std::string const &topicName);

  private:
	capnp::EzRpcClient rpcClient;
};

} // namespace registry
}; // namespace psf

