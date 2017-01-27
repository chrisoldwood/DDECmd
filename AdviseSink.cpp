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
#include "ValueFormatter.hpp"
#include <NCL/DDEConv.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AdviseSink::AdviseSink(tostream& out, const ValueFormatter& formatter, CEvent& abortEvent)
	: m_out(out)
	, m_formatter(formatter)
	, m_abortEvent(abortEvent)
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
	tstring    server = link->Conversation()->Service().c_str();
	tstring    topic = link->Conversation()->Topic().c_str();
	tstring    item = link->Item().c_str();
	TextFormat stringFormat = (link->Format() != CF_UNICODETEXT) ? ANSI_TEXT : UNICODE_TEXT;
	tstring    stringValue = value->GetString(stringFormat).c_str();

	m_out << m_formatter.format(server, topic, item, stringValue) << std::endl;

	m_out.flush();
}


////////////////////////////////////////////////////////////////////////////////
//! Handle the conversation closing.

void AdviseSink::OnDisconnect(CDDECltConv* /*conv*/)
{
	m_abortEvent.Signal();
}
