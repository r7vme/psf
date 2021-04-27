#include <capnp/ez-rpc.h>
#include <iostream>
#include <stdexcept>

#include "psf/registry/RegistryServer.h"

using namespace psf::registry;

int main()
{
	try
	{
		capnp::EzRpcServer server(kj::heap<RegistryServer>(), psf::getRegistrySocketFromEnvVar());
		kj::NEVER_DONE.wait(server.getWaitScope());
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
