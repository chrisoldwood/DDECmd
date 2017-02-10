////////////////////////////////////////////////////////////////////////////////
//! \file   MockDDECltConv.hpp
//! \brief  The MockDDECltConv class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_MOCKDDECLTCONV_HPP
#define NCL_MOCKDDECLTCONV_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <NCL/DDEFwd.hpp>
#include <NCL/IDDECltConv.hpp>
#include "ServiceTopic.hpp"

////////////////////////////////////////////////////////////////////////////////
//! A mock DDE client conversation.

class MockDDECltConv
#ifdef USE_DDE_INTERFACES
                    : public DDE::IDDECltConv
#endif
{
public:
	//! Constructor.
	MockDDECltConv(DDE::IDDEClient* client, ServiceTopicPtr conversation);

	//! Destructor.
	virtual ~MockDDECltConv();
	
	//
	// IDDEConv Properties.
	//

	//! The name of the DDE service.
	virtual const CString& Service() const;

	//! The name of the DDE topic.
	virtual const CString& Topic() const;

	//
	// IDDECltConv Properties.
	//

	//! The parent DDE client.
	virtual DDE::IDDEClient* Client() const;

	//! Set the timeout for a DDE transaction.
	virtual void SetTimeout(DWORD timeout);

	//
	// Methods.
	//

	//! Request a string based value.
	virtual CString RequestString(const tchar* pszItem, uint nFormat) const;

	//! Request a value in a custom format.
	virtual CDDEData Request(const tchar* pszItem, uint nFormat) const;

	//! Execute a string based command on the server.
	virtual void ExecuteString(const tchar* pszCommand) const;

	//! Execute a command on the server.
	virtual void Execute(const void* pValue, size_t nSize) const;

	//! Poke a sring based value.
	virtual void PokeString(const tchar* pszItem, const tchar* pszValue, uint nFormat) const;

	//! Poke a sring based value.
	virtual void Poke(const tchar* pszItem, uint nFormat, const void* pValue, size_t nSize) const;

	//! Start an advise loop on the server for an item.
	virtual CDDELink* CreateLink(const tchar* pszItem, uint nFormat = CF_TEXT);

private:
	//
	// Members.
	//
	DDE::IDDEClient*	m_client;		//!< The parent DDE client.
	CString				m_service;		//!< The name of the DDE service.
	CString				m_topic;		//!< The name of the DDE topic.
};

#endif // NCL_MOCKDDECLTCONV_HPP
