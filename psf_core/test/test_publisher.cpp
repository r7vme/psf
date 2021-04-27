#include "psf/Publisher.h"
#include <catch2/catch.hpp>

using namespace psf;

TEST_CASE("sets correct topicName", "[endpoint container]")
{
	EndpointContainer c("topic1");
	REQUIRE(c.topicName == "topic1");
}
