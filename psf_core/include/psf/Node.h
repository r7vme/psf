#pragma once
#include <atomic>
#include <memory>
#include <vector>

#include "psf/Publisher.h"
#include "psf/Subscriber.h"
#include "psf/common.h"
#include "psf/registry/RegistryClient.h"

namespace psf
{
class Node
{
  public:
	using SpinnerFunc = std::function<void()>;

	explicit Node(std::string name);
	~Node();
	void setSpinnerFunc(SpinnerFunc &&s);
	void spin();
	void stop();
	void waitData();

	template <typename MsgT>
	std::shared_ptr<Subscriber<MsgT>> createSubscriber(std::string const &topicName);
	template <typename MsgT>
	std::shared_ptr<Publisher<MsgT>> createPublisher(std::string const &topicName);

	Node(const Node &) = delete;
	Node &operator=(const Node &) = delete;
	Node(Node &&) = delete;
	Node &operator=(Node &&) = delete;

  private:
	std::string name;
	IpAddr ipAddress;
	std::shared_ptr<IOContext> ioContext;
	std::string registrySocket;
	SpinnerFunc spinnerFunc;
	std::atomic_flag notifyOnDataFlag{false};
	std::atomic_bool isStopped{false};

	std::vector<std::shared_ptr<EndpointContainer>> endpointContainersForPublishers;

	std::thread ioThread;
	std::thread registryThread;
};

template <typename MsgT>
std::shared_ptr<Subscriber<MsgT>> Node::createSubscriber(std::string const &topicName)
{
	UdpSocket socket(*ioContext, Endpoint(ipAddress, 0));

	// register subscriber
	auto ip = socket.local_endpoint().address().to_v4().to_uint();
	auto port = socket.local_endpoint().port();
	registry::RegistryClient registryClient(registrySocket);
	registryClient.registerSubscriber(name, topicName, ip, port);

	return std::make_shared<Subscriber<MsgT>>(std::move(socket), notifyOnDataFlag);
};

template <typename MsgT>
std::shared_ptr<Publisher<MsgT>> Node::createPublisher(std::string const &topicName)
{
	UdpSocket socket(*ioContext, Endpoint(ipAddress, 0));

	endpointContainersForPublishers.emplace_back(std::make_shared<EndpointContainer>(topicName));

	return std::make_shared<Publisher<MsgT>>(endpointContainersForPublishers.back(), std::move(socket), ioContext);
};

} // namespace psf
