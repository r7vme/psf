#include <algorithm>
#include <cassert>

#include "psf/common.h"
#include "psf/registry/Registry.h"

namespace psf::registry
{

bool Registry::registerEndpoint(Endpoint const &ep)
{
	bool ret{false};

	auto duplicate = std::find_if(std::begin(endpoints), std::end(endpoints), [&ep](Endpoint const &e) {
		return (e.address == ep.address) && (e.port == ep.port);
	});
	bool isExist = (duplicate != std::end(endpoints));

	if (!isExist)
	{
		endpoints.push_back(ep);
		psf::log("registered endpoint");
		ret = true;
	}

	return ret;
}

Endpoints Registry::getSubscribersForTopic(std::string const &topicName) const
{
	Endpoints subscribersForTopic{};

	std::copy_if(endpoints.begin(), endpoints.end(), std::back_inserter(subscribersForTopic),
				 [&topicName](Endpoint const &e) {
					 return ((e.topicName == topicName) && (e.type == Endpoint::Type::Subscriber));
				 });
	return subscribersForTopic;
}

} // namespace psf::registry
