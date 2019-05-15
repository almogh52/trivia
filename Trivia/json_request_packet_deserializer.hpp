#pragma once
#include <string>

#include "json.hpp"
using nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	template <typename T>
	static T DeserializePacket(std::vector<char>& buf);
};

template<typename T>
inline T JsonRequestPacketDeserializer::DeserializePacket(std::vector<char>& buf)
{
	// Parse the json
	json j = json::parse(buf);

	// Convert the json to the packet using the from_json function
	T packet = j.get<T>();

	return packet;
}
