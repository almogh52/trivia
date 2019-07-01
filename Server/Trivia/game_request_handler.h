#pragma once
#include "request_handler_interface.h"

#include "logged_user.h"
#include "game_manager.h"

class RequestHandlerFactory;

class GameRequestHandler :
	public IRequestHandler
{
public:
	GameRequestHandler(unsigned int gameId, const LoggedUser& user, std::shared_ptr<RequestHandlerFactory> handlerFactory, std::shared_ptr<GameManager> gameManager);

	virtual bool isRequestRelevant(const Request& req) const;
	virtual RequestResult handleRequest(const Request& req) const;

	virtual void disconnect() const;

private:
	RequestResult getQuestion(const Request& req) const;
	RequestResult submitAnswer(const Request& req) const;
	RequestResult leaveGame(const Request& req) const;

	unsigned int m_game;
	LoggedUser m_user;
	std::shared_ptr<GameManager> m_gameManager;
	std::shared_ptr<RequestHandlerFactory> m_handlerFactory;
};

