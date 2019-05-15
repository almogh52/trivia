#include "communicator.h"

Communicator::Communicator(std::shared_ptr<RequestHandlerFactory> handlerFactory) : m_handleFactory(handlerFactory)
{
}
