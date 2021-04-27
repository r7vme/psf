#pragma once
#include <memory>

#include "psf/registry/Registry.h"
#include "psf/registry/RegistryRpc.capnp.h"

namespace psf
{
namespace registry
{

class RegistryServer final : public rpc::Registry::Server
{
  public:
	RegistryServer() : registryImpl(){};

	kj::Promise<void> registerEndpoint(RegisterEndpointContext context) override;
	kj::Promise<void> getSubscriberEndpointsForTopic(GetSubscriberEndpointsForTopicContext context) override;

  private:
	RegistryServer(RegistryServer const &other) = delete;
	RegistryServer(RegistryServer &&other) = delete;
	RegistryServer &operator=(RegistryServer const &other) = delete;
	RegistryServer &operator=(RegistryServer &&other) = delete;

	Registry registryImpl;
};

} // namespace registry
} // namespace psf
