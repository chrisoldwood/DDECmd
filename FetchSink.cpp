////////////////////////////////////////////////////////////////////////////////
//! \file   FetchSink.cpp
//! \brief  The FetchSink class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "FetchSink.hpp"
#include <NCL/DDELink.hpp>
#include <NCL/DDEData.hpp>
#include <WCL/StringIO.hpp>
#include <Core/StringUtils.hpp>
#include "ValueFormatter.hpp"
#include <NCL/DDEConv.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

FetchSink::FetchSink(const ValueFormatter& formatter, Values& values)
	: m_formatter(formatter)
	, m_values(values)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

FetchSink::~FetchSink()
{
}


////////////////////////////////////////////////////////////////////////////////
//! Handle a link being updated.

void FetchSink::OnAdvise(CDDELink* link, const CDDEData* value)
{
	tstring    server = link->Conversation()->Service().c_str();
	tstring    topic = link->Conversation()->Topic().c_str();
	tstring    item = link->Item().c_str();
	TextFormat stringFormat = (link->Format() != CF_UNICODETEXT) ? ANSI_TEXT : UNICODE_TEXT;
	tstring    stringValue = value->GetString(stringFormat).c_str();
	
	m_values[item] = m_formatter.format(server, topic, item, stringValue);
}
