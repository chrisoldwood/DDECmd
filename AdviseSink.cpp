////////////////////////////////////////////////////////////////////////////////
//! \file   AdviseSink.cpp
//! \brief  The AdviseSink class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AdviseSink.hpp"
#include <NCL/DDELink.hpp>
#include <NCL/DDEData.hpp>
#include <WCL/StringIO.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

AdviseSink::AdviseSink(tostream& out)
	: m_out(out)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AdviseSink::~AdviseSink()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a link being updated.

void AdviseSink::OnAdvise(CDDELink* link, const CDDEData* value)
{
	if (link->Format() != CF_UNICODETEXT)
		m_out << value->GetString(ANSI_TEXT) << std::endl;
	else
		m_out << value->GetString(UNICODE_TEXT) << std::endl;

	m_out.flush();
}
