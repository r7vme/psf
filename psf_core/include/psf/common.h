#pragma once
#include <asio.hpp>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

namespace psf
{
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using IpAddr = asio::ip::address_v4;
using Endpoint = asio::ip::udp::endpoint;
using UdpSocket = asio::ip::udp::socket;
using IOContext = asio::io_context;

// helper functions
inline std::string getEnvVarOrDefault(std::string const &varName, std::string const &defaultValue)
{
	const char *var = std::getenv(varName.c_str());
	if (var)
	{
		return std::string(var);
	}
	return defaultValue;
}

inline std::string getRegistrySocketFromEnvVar()
{
	return getEnvVarOrDefault("PSF_REGISTRY", "127.0.0.1:11311");
}

inline std::string getListenIpFromEnvVar()
{
	return getEnvVarOrDefault("PSF_LISTEN_IP", "127.0.0.1");
}

inline void log(const char *msg)
{
	std::cout << msg << std::endl;
}

} // namespace psf
