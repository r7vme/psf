#pragma once
#include <capnp/message.h>
#include <capnp/serialize.h>
#include <cstddef>
#include <memory>
#include <vector>

#include "psf/common.h"
#include "psf/registry/RegistryClient.h"

namespace psf
{

// TODO: proper thread-safe container
struct EndpointContainer {
	EndpointContainer(std::string topicName) : topicName(std::move(topicName))
	{
	}
	std::string const topicName;
	std::vector<Endpoint> endpoints;
	std::mutex mtx;
};

template <typename MsgT>
class Publisher
{
  public:
	Publisher(std::shared_ptr<EndpointContainer> endpointContainer, UdpSocket &&socket, std::shared_ptr<IOContext> io)
		: endpointContainer(endpointContainer), socket(std::move(socket)), io(io),
		  buffer(kj::heapArray<capnp::word>(capnp::sizeInWords<MsgT>() + 1))
	{
		std::memset(buffer.asBytes().begin(), 0, buffer.asBytes().size());
	};

	void publish(typename MsgT::Builder msgBuilder);
	size_t subscribersNum();

  private:
	static_assert(capnp::_::structSize<MsgT>().pointers == 0,
				  "TODO: Messages with pointers are not supported. Please avoid :List, :Text and nesting.");
	void handlePublish();

	std::shared_ptr<EndpointContainer> endpointContainer;
	UdpSocket socket;
	std::shared_ptr<IOContext> io;
	kj::Array<capnp::word> buffer;
	std::mutex bufferMutex;
};

template <typename MsgT>
size_t Publisher<MsgT>::subscribersNum()
{
	std::lock_guard<std::mutex> lock(endpointContainer->mtx);
	return endpointContainer->endpoints.size();
}

template <typename MsgT>
void Publisher<MsgT>::handlePublish()
{
	std::lock_guard<std::mutex> endpointLock(endpointContainer->mtx);
	if (!endpointContainer->endpoints.empty())
	{
		std::lock_guard<std::mutex> bufferLock(bufferMutex);
		auto bytes = buffer.asBytes();
		for (auto &endpoint : endpointContainer->endpoints)
		{
			socket.send_to(asio::buffer(bytes.begin(), bytes.size()), endpoint);
		}
	}
}

template <typename MsgT>
void Publisher<MsgT>::publish(typename MsgT::Builder msgBuilder)
{
	auto msgBuffer = capnp::writeDataStruct(msgBuilder);
	assert(msgBuffer.size() == buffer.asBytes().size());

	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		std::copy(msgBuffer.asBytes().begin(), msgBuffer.asBytes().end(), buffer.asBytes().begin());
	}

	io->post([this] { handlePublish(); });
}

} // namespace psf
