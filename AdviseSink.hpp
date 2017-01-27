////////////////////////////////////////////////////////////////////////////////
//! \file   AdviseSink.hpp
//! \brief  The AdviseSink class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_ADVISESINK_HPP
#define APP_ADVISESINK_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/tiosfwd.hpp>
#include <NCL/DefDDEClientListener.hpp>
#include <WCL/Event.hpp>

// Forward declarations.
class ValueFormatter;

////////////////////////////////////////////////////////////////////////////////
//!	The sink used to handle updates from the DDE server when just advising.

class AdviseSink : public CDefDDEClientListener
{
public:
	//! Constructor.
	AdviseSink(tostream& out, const ValueFormatter& formatter, CEvent& abortEvent);

	//! Destructor.
	~AdviseSink();
	
private:
	//
	// Members.
	//
	tostream&				m_out;			//!< The output stream.
	const ValueFormatter&	m_formatter;	//!< The value formatter.
	CEvent&					m_abortEvent;	//!< Event used to signal termination.

	//
	// IDDEClientListener Methods.
	//

	//! Handle a link being updated.
	virtual void OnAdvise(CDDELink* link, const CDDEData* value);

	//! Handle the conversation closing.
	virtual void OnDisconnect(CDDECltConv* conv);
};

#endif // APP_ADVISESINK_HPP
