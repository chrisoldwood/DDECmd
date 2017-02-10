////////////////////////////////////////////////////////////////////////////////
//! \file   MockDDEClient.cpp
//! \brief  The MockDDEClient class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "MockDDEClient.hpp"
#include <WCL/StrArray.hpp>
#include "MockDDECltConv.hpp"

using namespace DDE;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

MockDDEClient::MockDDEClient()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

MockDDEClient::~MockDDEClient()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Create a conversation for the service and topic.

DDE::IDDECltConv* MockDDEClient::CreateConversation(const tchar* service, const tchar* topic)
{
	ServiceTopicPtr conversation = ServiceTopicPtr(new ServiceTopic(service, topic));

	m_established.push_back(conversation);

#ifdef USE_DDE_INTERFACES
	return new MockDDECltConv(this, conversation);
#else
	return nullptr;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Close the conversation.

void MockDDEClient::DestroyConversation(DDE::IDDECltConv* conversation)
{
#ifdef USE_DDE_INTERFACES
	delete conversation;
#else
	conversation = nullptr;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Query for all running servers and topics.

void MockDDEClient::QueryAll(CStrArray& servers, CStrArray& topics) const
{
	typedef ServiceTopics::const_iterator const_iter;

	for (const_iter it = m_runningServers.begin(); it != m_runningServers.end(); ++it)
	{
		ServiceTopicPtr conversation = *it;

		servers.Add(conversation->m_server.c_str());
		topics.Add(conversation->m_topic.c_str());
	}
}
