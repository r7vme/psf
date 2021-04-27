#pragma once
#include <asio.hpp>
#include <atomic>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include <cstddef>

#include "psf/common.h"
#include "psf/registry/RegistryClient.h"

namespace psf
{
template <typename MsgT>
class Subscriber
{
  public:
	Subscriber(UdpSocket &&socket, std::atomic_flag &notifyOnDataFlag);

	typename MsgT::Reader getMsgReader();

  private:
	static_assert(capnp::_::structSize<MsgT>().pointers == 0,
				  "TODO: Messages with pointers are not supported. Please avoid :List, :Text and nesting.");

	void doReceive();
	UdpSocket socket;
	std::atomic_flag &notifyOnDataFlag;
	kj::Array<capnp::word> internalBuffer;
	kj::Array<capnp::word> userBuffer;
	std::mutex bufferMutex;
};

template <typename MsgT>
Subscriber<MsgT>::Subscriber(UdpSocket &&socket, std::atomic_flag &notifyOnDataFlag)
	: socket(std::move(socket)), notifyOnDataFlag(notifyOnDataFlag),
	  internalBuffer(kj::heapArray<capnp::word>(capnp::sizeInWords<MsgT>() + 1)),
	  userBuffer(kj::heapArray<capnp::word>(capnp::sizeInWords<MsgT>() + 1))
{
	doReceive();
};

template <typename MsgT>
void Subscriber<MsgT>::doReceive()
{
	socket.async_wait(UdpSocket::wait_read, [this](std::error_code const &ec) {
		if (ec)
		{
			throw std::system_error(ec);
		}

		{
			std::lock_guard<std::mutex> lock(bufferMutex);
			while (socket.available() != 0u)
			{
				socket.receive(asio::buffer(internalBuffer.asBytes().begin(), internalBuffer.asBytes().size()));
			}
		}

		notifyOnDataFlag.test_and_set(std::memory_order_acquire);
		notifyOnDataFlag.notify_one();

		doReceive();
	});
};

template <typename MsgT>
typename MsgT::Reader Subscriber<MsgT>::getMsgReader()
{
	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		std::copy(internalBuffer.asBytes().begin(), internalBuffer.asBytes().end(), userBuffer.asBytes().begin());
	}
	return capnp::readDataStruct<MsgT>(userBuffer);
}

} // namespace psf
