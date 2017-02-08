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
#include <vector>

////////////////////////////////////////////////////////////////////////////////
//! A fake DDE client for use in unit tests.

class MockDDEClient
#ifdef USE_DDE_INTERFACES
                    : public DDE::IDDEClient
#endif
{
public:
	//! Default constructor.
	MockDDEClient();

	//! Destructor.
	virtual ~MockDDEClient();
	
	std::vector<tstring> m_servers;
	std::vector<tstring> m_topics;

	//
	// IDDEClient methods.
	//

	//! Create a conversation for the service and topic.
	virtual CDDECltConv* CreateConversation(const tchar* service, const tchar* topic);

	//! Query for all running servers and topics.
	virtual void QueryAll(CStrArray& servers, CStrArray& topics) const;
};

#endif // APP_MOCKDDECLIENT_HPP
