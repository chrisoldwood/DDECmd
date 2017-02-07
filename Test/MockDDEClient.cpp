////////////////////////////////////////////////////////////////////////////////
//! \file   MockDDEClient.cpp
//! \brief  The MockDDEClient class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "MockDDEClient.hpp"
#include <WCL/StrArray.hpp>

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

CDDECltConv* MockDDEClient::CreateConversation(const tchar* /*service*/, const tchar* /*topic*/)
{
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
//! Query for all running servers and topics.

void MockDDEClient::QueryAll(CStrArray& servers, CStrArray& topics) const
{
	typedef std::vector<tstring>::const_iterator const_iter;

	for (const_iter it = m_servers.begin(); it != m_servers.end(); ++it)
		servers.Add(it->c_str());

	for (const_iter it = m_topics.begin(); it != m_topics.end(); ++it)
		topics.Add(it->c_str());
}
