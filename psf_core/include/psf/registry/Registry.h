#pragma once
#include <string>
#include <vector>

#include "psf/common.h"

namespace psf
{
namespace registry
{

struct Endpoint {
	enum class Type : uint16 { Publisher, Subscriber };
	Type type;
	uint32 address{0};
	uint16 port{0};
	std::string nodeName{};
	std::string topicName{};
};

using Endpoints = std::vector<Endpoint>;

class Registry final
{
  public:
	Registry(){};
	bool registerEndpoint(Endpoint const &ep);
	Endpoints getSubscribersForTopic(std::string const &topic) const;

  private:
	Registry(Registry const &other) = delete;
	Registry(Registry &&other) = delete;
	Registry &operator=(Registry const &other) = delete;
	Registry &operator=(Registry &&other) = delete;

	Endpoints endpoints;
};

} // namespace registry
} // namespace psf
