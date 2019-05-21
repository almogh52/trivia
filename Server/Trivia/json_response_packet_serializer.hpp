#pragma once
#include <string>
#include <vector>

#include "json.hpp"
using nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	template <typename T>
	static std::vector<char> SerializePacket(T& packet);
};

template<typename T>
inline std::vector<char> JsonResponsePacketSerializer::SerializePacket(T & packet)
{
    std::string jsonStr;

    // Convert the packet to json using the to_json function
    json j = packet;

    // Convert the json to string
    jsonStr = j.dump();

    // Convert to vector of byte
    return std::vector<char>(jsonStr.begin(), jsonStr.end());
}