////////////////////////////////////////////////////////////////////////////////
//! \file   FetchCmd.cpp
//! \brief  The FetchCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "FetchCmd.hpp"
#include <Core/tiostream.hpp>
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <Core/InvalidArgException.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include <WCL/Clipboard.hpp>
#include <Core/StringUtils.hpp>
#include <WCL/ConsoleApp.hpp>
#include "DDECmd.hpp"
#include "FetchSink.hpp"
#include <NCL/DDELink.hpp>
#include "ValueFormatter.hpp"
#include <NCL/DDEData.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] =
{
	{ USAGE,	TXT("?"),	nullptr,		Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		nullptr,		TXT("Display the command syntax")	},
	{ USAGE,	nullptr,	TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		nullptr,		TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ ITEM,		TXT("i"),	TXT("item"), 	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::MULTIPLE,	TXT("item"),	TXT("The item name(s)")				},
	{ FORMAT,	TXT("f"),	TXT("format"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("format"),	TXT("The clipboard format to use")	},
	{ LINK,		TXT("l"),	TXT("link"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("link"),	TXT("The DDE link")					},
	{ NO_TRIM,	TXT("nt"),	TXT("no-trim"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		nullptr,		TXT("Don't trim whitespace")		},
	{ OUT_FMT,	TXT("of"),	TXT("output-format"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("format"),	TXT("The output format")			},
	{ DATE_FMT,	TXT("df"),	TXT("date-format"),		Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("format"),	TXT("The timestamp date format")	},
	{ TIME_FMT,	TXT("tf"),	TXT("time-format"),		Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("format"),	TXT("The timestamp time format")	},
	{ TIMEOUT,	nullptr,	TXT("timeout"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("timeout"),	TXT("The timeout (ms) for the DDE reply")	},
	{ DELAY,	nullptr,	TXT("delay"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("delay"),	TXT("The delay (ms) between advise & request")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

FetchCmd::FetchCmd(int argc, tchar* argv[], WCL::ConsoleApp& app)
	: WCL::ConsoleCmd(s_switches, s_switches+s_switchCount, argc, argv, USAGE)
	, m_app(app)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

FetchCmd::~FetchCmd()
{
}
////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* FetchCmd::getDescription()
{
	return TXT("Fetch the value of one or more items");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* FetchCmd::getUsage()
{
	return TXT("USAGE: DDECmd fetch [--server <server> --topic <topic> --item <item> ... | --link <link>] [--format <format>]");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int FetchCmd::doExecute(tostream& out, tostream& /*err*/)
{
	ASSERT(m_parser.getUnnamedArgs().at(0) == TXT("fetch"));

	// Type aliases.
	typedef std::vector<tstring> Items;
	typedef Core::CmdLineParser::StringVector::const_iterator ItemConstIter;
	typedef FetchSink::Values::const_iterator ValueConstIter;

	tstring server;
	tstring topic;
	Items   items;

	// Validate and extract the command line arguments.
	if (m_parser.isSwitchSet(LINK))
	{
		if (m_parser.isSwitchSet(SERVER) || m_parser.isSwitchSet(TOPIC) || m_parser.isSwitchSet(ITEM))
			throw Core::CmdLineException(TXT("The --link switch cannot be mixed with --server, --topic & --item"));

		tstring link = m_parser.getSwitchValue(LINK);
		tstring item;

		if (!CDDELink::ParseLink(link, server, topic, item))
			throw Core::InvalidArgException(Core::fmt(TXT("Invalid DDE link format '%s'"), link.c_str()));

		items.push_back(item);
	}
	else
	{
		ASSERT(!m_parser.isSwitchSet(LINK));

		if (!m_parser.isSwitchSet(SERVER))
			throw Core::CmdLineException(TXT("No DDE server name specified [--server]"));

		if (!m_parser.isSwitchSet(TOPIC))
			throw Core::CmdLineException(TXT("No DDE server topic specified [--topic]"));

		if (!m_parser.isSwitchSet(ITEM))
			throw Core::CmdLineException(TXT("No item(s) specified [--item]"));

		server = m_parser.getSwitchValue(SERVER);
		topic  = m_parser.getSwitchValue(TOPIC);
		items  = m_parser.getNamedArgs().find(ITEM)->second;
	}

	tstring formatName = TXT("CF_TEXT");

	if (m_parser.isSwitchSet(FORMAT))
		formatName = m_parser.getSwitchValue(FORMAT);

	uint format = CClipboard::FormatHandle(formatName.c_str());

	if (format == CF_NONE)
		throw Core::InvalidArgException(Core::fmt(TXT("Invalid clipboard format '%s'"), formatName.c_str()));

	const bool trimValue = !m_parser.isSwitchSet(NO_TRIM);
	tstring    valueFormat = (items.size() == 1) ? ValueFormatter::DEFAULT_SINGLE_ITEM_FORMAT
												 : ValueFormatter::DEFAULT_MULTI_ITEM_FORMAT;

	if (m_parser.isSwitchSet(OUT_FMT))
		valueFormat = m_parser.getSwitchValue(OUT_FMT);

	tstring dateFormat = ValueFormatter::DEFAULT_DATE_FORMAT;

	if (m_parser.isSwitchSet(DATE_FMT))
		dateFormat = m_parser.getSwitchValue(DATE_FMT);

	tstring timeFormat = ValueFormatter::DEFAULT_TIME_FORMAT;

	if (m_parser.isSwitchSet(TIME_FMT))
		timeFormat = m_parser.getSwitchValue(TIME_FMT);

	const ValueFormatter formatter(valueFormat, trimValue, dateFormat, timeFormat);

	// Test the formatter to check for errors in the format string.
	formatter.format(TXT(""), TXT(""), TXT(""), TXT(""));

	DWORD timeout = 0;

	if (m_parser.isSwitchSet(TIMEOUT))
		timeout = Core::parse<DWORD>(m_parser.getSwitchValue(TIMEOUT));

	DWORD delay = 0;

	if (m_parser.isSwitchSet(DELAY))
		delay = Core::parse<DWORD>(m_parser.getSwitchValue(DELAY));

	// Open the conversation.
	CDDEClient client;
	DDE::CltConvPtr conv(client.CreateConversation(server.c_str(), topic.c_str()));

	if (timeout != 0)
		conv->SetTimeout(timeout);

	FetchSink::Values values;

	// Start listening for updates.
	FetchSink sink(formatter, values);
	client.AddListener(&sink);

	// Create the links...
	for (ItemConstIter it = items.begin(); it != items.end(); ++it)
	{
		const tstring& item = *it;

		conv->CreateLink(item.c_str(), format);
	}

	CMsgThread& thread = m_app.mainThread();
	CEvent&     abortEvent = m_app.getAbortEvent();

	// Briefly pump messages to catch any advised values.
	DWORD now = ::GetTickCount();
	DWORD expireAt = now + delay;

	while (!abortEvent.IsSignalled() && thread.ProcessMsgQueue() && (::GetTickCount() < expireAt))
		;

	// Request the remaining items.
	for (ItemConstIter it = items.begin(); it != items.end(); ++it)
	{
		const tstring& item = *it;

		if (values.find(item) == values.end())
		{
			CDDEData   ddeValue = conv->Request(item.c_str(), format);
			TextFormat stringFormat = (format != CF_UNICODETEXT) ? ANSI_TEXT : UNICODE_TEXT;
			tstring    stringValue = ddeValue.GetString(stringFormat).c_str();

			values[item] = formatter.format(server, topic, item, stringValue);
		}
	}

	// Write the values...
	for (ValueConstIter it = values.begin(); it != values.end(); ++it)
	{
		//const tstring& item = it->first;
		const tstring& value = it->second;

		out << value << std::endl;
	}

	return EXIT_SUCCESS;
}
