////////////////////////////////////////////////////////////////////////////////
//! \file   AdviseSink.cpp
//! \brief  The AdviseSink class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AdviseSink.hpp"
#include <NCL/DDELink.hpp>
#include <NCL/DDEData.hpp>
#include <WCL/StringIO.hpp>
#include <Core/StringUtils.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

AdviseSink::AdviseSink(tostream& out, bool labelValues)
	: m_out(out)
	, m_labelValues(labelValues)
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
	TextFormat stringFormat = (link->Format() != CF_UNICODETEXT) ? ANSI_TEXT : UNICODE_TEXT;
	tstring    stringValue = value->GetString(stringFormat).c_str();

	Core::trim(stringValue);

	if (m_labelValues)
		m_out << link->Item() << ": ";

	m_out << stringValue << std::endl;

	m_out.flush();
}
