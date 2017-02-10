////////////////////////////////////////////////////////////////////////////////
//! \file   MockDDECltConv.cpp
//! \brief  The MockDDECltConv class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "MockDDECltConv.hpp"
#include <NCL/DDEData.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

MockDDECltConv::MockDDECltConv(DDE::IDDEClient* client, ServiceTopicPtr conversation)
	: m_client(client)
	, m_service(conversation->m_server.c_str())
	, m_topic(conversation->m_topic.c_str())
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

MockDDECltConv::~MockDDECltConv()
{
}

////////////////////////////////////////////////////////////////////////////////
//! The name of the DDE service.

const CString& MockDDECltConv::Service() const
{
	return m_service;
}

////////////////////////////////////////////////////////////////////////////////
//! The name of the DDE topic.

const CString& MockDDECltConv::Topic() const
{
	return m_topic;
}

////////////////////////////////////////////////////////////////////////////////
//! The parent DDE client.

DDE::IDDEClient* MockDDECltConv::Client() const
{
	return m_client;
}

////////////////////////////////////////////////////////////////////////////////
//! Set the timeout for a DDE transaction.

void MockDDECltConv::SetTimeout(DWORD /*timeout*/)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Request a string based value.

CString MockDDECltConv::RequestString(const tchar* /*pszItem*/, uint /*nFormat*/) const
{
	return TXT("");
}

////////////////////////////////////////////////////////////////////////////////
//! Request a value in a custom format.

CDDEData MockDDECltConv::Request(const tchar* /*pszItem*/, uint /*nFormat*/) const
{
	return *((CDDEData*)nullptr);
}

////////////////////////////////////////////////////////////////////////////////
//! Execute a string based command on the server.

void MockDDECltConv::ExecuteString(const tchar* /*pszCommand*/) const
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute a command on the server.

void MockDDECltConv::Execute(const void* /*pValue*/, size_t /*nSize*/) const
{
}

////////////////////////////////////////////////////////////////////////////////
//! Poke a sring based value.

void MockDDECltConv::PokeString(const tchar* /*pszItem*/, const tchar* /*pszValue*/, uint /*nFormat*/) const
{
}

////////////////////////////////////////////////////////////////////////////////
//! Poke a sring based value.

void MockDDECltConv::Poke(const tchar* /*pszItem*/, uint /*nFormat*/, const void* /*pValue*/, size_t /*nSize*/) const
{
}

////////////////////////////////////////////////////////////////////////////////
//! Start an advise loop on the server for an item.

CDDELink* MockDDECltConv::CreateLink(const tchar* /*pszItem*/, uint /*nFormat*/)
{
	return nullptr;
}
