////////////////////////////////////////////////////////////////////////////////
//! \file   FetchSink.hpp
//! \brief  The FetchSink class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_FETCHSINK_HPP
#define APP_FETCHSINK_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <map>
#include <NCL/DefDDEClientListener.hpp>

// Forward declarations.
class ValueFormatter;

////////////////////////////////////////////////////////////////////////////////
//! The sink used to handle updates from the DDE server when fetching values.

class FetchSink : public CDefDDEClientListener
{
public:
	// Type aliases.
	typedef std::map<tstring, tstring> Values;

public:
	//! Constructor.
	FetchSink(const ValueFormatter& formatter, Values& values);

	//! Destructor.
	~FetchSink();
	
private:
	//
	// Members.
	//
	const ValueFormatter&	m_formatter;	//!< The value formatter.
	Values&					m_values;		//!< The values for advised links.

	//
	// IDDEClientListener Methods.
	//

	//! Handle a link being updated.
	virtual void OnAdvise(CDDELink* link, const CDDEData* value);
};

#endif // APP_FETCHSINK_HPP
