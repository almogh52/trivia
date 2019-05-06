#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	template <typename T>
	static std::string SerializePacket(T& packet);
};

template<typename T>
inline std::string JsonResponsePacketSerializer::SerializePacket(T & packet)
{
	// Convert the packet to json using the to_json function
	json j = packet;

	// Convert the json to string
	return j.dump();
}