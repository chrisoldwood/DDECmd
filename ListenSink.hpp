////////////////////////////////////////////////////////////////////////////////
//! \file   ListenSink.hpp
//! \brief  The ListenSink class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_LISTENSINK_HPP
#define APP_LISTENSINK_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/tiosfwd.hpp>
#include <NCL/DefDDEServerListener.hpp>
#include <map>

////////////////////////////////////////////////////////////////////////////////
//! The sink used to handle requests for the DDE server.

class ListenSink : public CDefDDEServerListener
{
public:
	//! A map of DDE link to value.
	typedef std::map<CDDELink*, tstring> LinkValues;

public:
	//! Constructor.
	ListenSink(const tstring& server, const tstring& topic, tostream& out, DWORD delay, LinkValues& values);

	//! Destructor.
	~ListenSink();

private:
	//
	// Members.
	//
	tstring		m_server;	//!< The server name.
	tstring		m_topic;	//!< The topic name.
	tostream&	m_out;		//!< The output stream.
	DWORD		m_delay;	//!< The delay before replying.
	LinkValues&	m_values;	//!< The current value for each link.

	//
	// IDDEServerListener Methods.
	//

	//! Query for all services and topics supported.
	virtual bool OnWildConnect(CStrArray& services, CStrArray& topics);

	//! Query for all topics supported by a service.
	virtual bool OnWildConnectService(const tchar* service, CStrArray& topics);

	//! Query for all services supporting a topic.
	virtual bool OnWildConnectTopic(const tchar* topic, CStrArray& services);

	//! Request to connect on the named topic.
	virtual bool OnConnect(const tchar* service, const tchar* topic);

	//! Conversation established.
	virtual void OnConnectConfirm(CDDESvrConv* conv);

	//! Conversation terminated.
	virtual void OnDisconnect(CDDESvrConv* conv);

	//! Value requested for an item.
	virtual bool OnRequest(CDDESvrConv* conv, const tchar* item, uint format, CDDEData& data);

	//! Start an advise loop on an item.
	virtual bool OnAdviseStart(CDDESvrConv* conv, const tchar* item, uint format);

	//! Request the value for an item being advised on.
	virtual bool OnAdviseRequest(CDDESvrConv* conv, CDDELink* link, CDDEData& data);

	//! Stop the advise loop or an item.
	virtual void OnAdviseStop(CDDESvrConv* conv, CDDELink* link);

	//! Execute the command.
	virtual bool OnExecute(CDDESvrConv* conv, const CString& command);

	//! Write a value to an item.
	virtual bool OnPoke(CDDESvrConv* conv, const tchar* item, uint format, const CDDEData& data);
};

#endif // APP_LISTENSINK_HPP
