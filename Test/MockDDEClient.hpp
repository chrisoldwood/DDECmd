////////////////////////////////////////////////////////////////////////////////
//! \file   MockDDEClient.hpp
//! \brief  The MockDDEClient class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_MOCKDDECLIENT_HPP
#define APP_MOCKDDECLIENT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <NCL/DDEFwd.hpp>
#include <NCL/IDDEClient.hpp>
#include "ServiceTopic.hpp"
#include <vector>

////////////////////////////////////////////////////////////////////////////////
//! A fake DDE client for use in unit tests.

class MockDDEClient
#ifdef USE_DDE_INTERFACES
                    : public DDE::IDDEClient
#endif
{
public:
	//! A collection of conversations.
	typedef std::vector<ServiceTopicPtr> ServiceTopics;

public:
	//! Default constructor.
	MockDDEClient();

	//! Destructor.
	virtual ~MockDDEClient();
	
	ServiceTopics m_runningServers;
	ServiceTopics m_established;

	//
	// IDDEClient methods.
	//

	//! Create a conversation for the service and topic.
	virtual DDE::IDDECltConv* CreateConversation(const tchar* service, const tchar* topic);

	//! Close the conversation.
	virtual void DestroyConversation(DDE::IDDECltConv* conv);

	//! Query for all running servers and topics.
	virtual void QueryAll(CStrArray& servers, CStrArray& topics) const;
};

#endif // APP_MOCKDDECLIENT_HPP
