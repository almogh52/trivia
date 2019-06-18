#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(const std::string& message) : m_message(message) {}
	virtual ~Exception() noexcept = default;
	virtual const char* what() const noexcept { return m_message.c_str(); }

protected:
	std::string m_message;
};

