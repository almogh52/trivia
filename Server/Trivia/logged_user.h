#pragma once
#include <string>

class LoggedUser
{
public:
    LoggedUser(std::string username);

    std::string getUsername() const;

    bool operator==(const LoggedUser& user) const
    {
		return m_username == user.getUsername();
    }

private:
    std::string m_username;
};

namespace std {

	template <>
	struct hash<LoggedUser>
	{
		std::size_t operator()(const LoggedUser& user) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return hash<string>()(user.getUsername());
		}
	};

}

