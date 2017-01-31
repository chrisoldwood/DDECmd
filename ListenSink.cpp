////////////////////////////////////////////////////////////////////////////////
//! \file   ListenSink.cpp
//! \brief  The ListenSink class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ListenSink.hpp"
#include <NCL/DDESvrConv.hpp>
#include <NCL/DDEData.hpp>
#include <WCL/StrArray.hpp>
#include <ostream>
#include <WCL/Clipboard.hpp>
#include <WCL/StringIO.hpp>
#include <Core/AnsiWide.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ListenSink::ListenSink(const tstring& server, const tstring& topic, tostream& out, DWORD delay, LinkValues& values)
	: m_server(server)
	, m_topic(topic)
	, m_out(out)
	, m_delay(delay)
	, m_values(values)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ListenSink::~ListenSink()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Query for all services and topics supported.

bool ListenSink::OnWildConnect(CStrArray& services, CStrArray& topics)
{
	m_out << TXT("XTYP_WILDCONNECT")
	      << std::endl;

	services.Add(m_server.c_str());
	topics.Add(!m_topic.empty() ? m_topic.c_str() : TXT("*"));

//	if (m_delay != 0)
//		::Sleep(m_delay);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Query for all topics supported by a service.

bool ListenSink::OnWildConnectService(const tchar* service, CStrArray& topics)
{
	m_out << TXT("XTYP_WILDCONNECT: '")
	      << service << TXT("'")
	      << std::endl;

	topics.Add(!m_topic.empty() ? m_topic.c_str() : TXT("*"));

//	if (m_delay != 0)
//		::Sleep(m_delay);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Query for all services supporting a topic.

bool ListenSink::OnWildConnectTopic(const tchar* topic, CStrArray& services)
{
	m_out << TXT("XTYP_WILDCONNECT: '")
	      << topic << TXT("'")
	      << std::endl;

	services.Add(m_server.c_str());

//	if (m_delay != 0)
//		::Sleep(m_delay);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Request to connect on the named topic.

bool ListenSink::OnConnect(const tchar* service, const tchar* topic)
{
	m_out << TXT("XTYP_CONNECT: '")
	      << service << TXT("', '")
	      << topic << TXT("'")
	      << std::endl;

//	if (m_delay != 0)
//		::Sleep(m_delay);

	return ( (tstricmp(service, m_server.c_str()) == 0)
		  && ( (m_topic.empty()) || (tstricmp(topic, m_topic.c_str()) == 0) )
           );
}

////////////////////////////////////////////////////////////////////////////////
//! Conversation established.

void ListenSink::OnConnectConfirm(CDDESvrConv* conv)
{
	m_out << TXT("XTYP_CONNECT_CONFIRM: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("'")
	      << std::endl;

//	if (m_delay != 0)
//		::Sleep(m_delay);
}

////////////////////////////////////////////////////////////////////////////////
//! Conversation terminated.

void ListenSink::OnDisconnect(CDDESvrConv* conv)
{
	m_out << TXT("XTYP_DISCONNECT: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("'")
	      << std::endl;

//	if (m_delay != 0)
//		::Sleep(m_delay);
}

////////////////////////////////////////////////////////////////////////////////
//! Value requested for an item.

bool ListenSink::OnRequest(CDDESvrConv* conv, const tchar* item, uint format, CDDEData& data)
{
	m_out << TXT("XTYP_REQUEST: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("', '")
	      << item << TXT("', ")
	      << format << TXT(" [") << CClipboard::FormatName(format) << TXT("]")
	      << std::endl;

	if (format != CF_TEXT)
		return false;

	data.SetData("\0\0", 2, 0);

	if (m_delay != 0)
		::Sleep(m_delay);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Start an advise loop on an item.

bool ListenSink::OnAdviseStart(CDDESvrConv* conv, const tchar* item, uint format)
{
	m_out << TXT("XTYP_ADVSTART: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("', '")
	      << item << TXT("', ")
	      << format << TXT(" [") << CClipboard::FormatName(format) << TXT("]")
	      << std::endl;

	if (format != CF_TEXT)
		return false;

//	if (m_delay != 0)
//		::Sleep(m_delay);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Request the value for an item being advised on.

bool ListenSink::OnAdviseRequest(CDDESvrConv* conv, CDDELink* link, CDDEData& data)
{
	m_out << TXT("XTYP_ADVREQ: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("', '")
	      << link->Item().c_str() << TXT("', ")
	      << link->Format() << TXT(" [") << CClipboard::FormatName(link->Format()) << TXT("]")
	      << std::endl;

	if (link->Format() != CF_TEXT)
		return false;

	LinkValues::const_iterator it = m_values.find(link);

	if (it != m_values.end())
		data.SetAnsiString(CString(it->second.c_str()), ANSI_TEXT);
	else
		data.SetData("\0\0", 2, 0);

//	if (m_delay != 0)
//		::Sleep(m_delay);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Stop the advise loop or an item.

void ListenSink::OnAdviseStop(CDDESvrConv* conv, CDDELink* link)
{
	m_out << TXT("XTYP_ADVSTOP: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("', '")
	      << link->Item().c_str() << TXT("', '")
	      << link->Format() << TXT(" [") << CClipboard::FormatName(link->Format()) << TXT("]")
	      << std::endl;

//	if (m_delay != 0)
//		::Sleep(m_delay);
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

bool ListenSink::OnExecute(CDDESvrConv* conv, const CString& command)
{
	m_out << TXT("XTYP_EXECUTE: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("', '")
	      << command.c_str() << TXT("'")
	      << std::endl;

	if (m_delay != 0)
		::Sleep(m_delay);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! Write a value to an item.

bool ListenSink::OnPoke(CDDESvrConv* conv, const tchar* item, uint format, const CDDEData& data)
{
	m_out << TXT("XTYP_POKE: '")
	      << conv->Service().c_str() << TXT("', '")
	      << conv->Topic().c_str() << TXT("', '")
	      << item << TXT("', ")
	      << format << TXT(" [") << CClipboard::FormatName(format) << TXT("], ")
	      << data.Size() << TXT(" bytes")
	      << std::endl;

	if (format != CF_TEXT)
		return false;

	if (m_delay != 0)
		::Sleep(m_delay);

	return true;
}
