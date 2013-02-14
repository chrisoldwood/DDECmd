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

////////////////////////////////////////////////////////////////////////////////
//!	The sink used to handle updates from the DDE server.

class AdviseSink : public CDefDDEClientListener
{
public:
	//! Constructor.
	AdviseSink(tostream& out, bool labelValues);

	//! Destructor.
	~AdviseSink();
	
private:
	//
	// Members.
	//
	tostream&	m_out;			//!< The output stream.
	bool		m_labelValues;	//!< Display the item name.

	//
	// IDDEClientListener Methods.
	//

	//! Handle a link being updated.
	virtual void OnAdvise(CDDELink* link, const CDDEData* value);
};

#endif // APP_ADVISESINK_HPP
