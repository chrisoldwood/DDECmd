////////////////////////////////////////////////////////////////////////////////
//! \file   RequestCmd.cpp
//! \brief  The RequestCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "RequestCmd.hpp"
#include <Core/tiostream.hpp>
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <Core/InvalidArgException.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include <WCL/Clipboard.hpp>
#include <Core/StringUtils.hpp>
#include <NCL/DDEData.hpp>
#include <WCL/StringIO.hpp>
#include <NCL/DDELink.hpp>
#include "ValueFormatter.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] =
{
	{ USAGE,	TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ USAGE,	NULL,		TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ ITEM,		TXT("i"),	TXT("item"), 	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::MULTIPLE,	TXT("item"),	TXT("The item name(s)")				},
	{ FORMAT,	TXT("f"),	TXT("format"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("format"),	TXT("The clipboard format to use")	},
	{ LINK,		TXT("l"),	TXT("link"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("link"),	TXT("The DDE link")					},
	{ NO_TRIM,	TXT("nt"),	TXT("no-trim"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Don't trim whitespace")		},
	{ OUT_FMT,	TXT("of"),	TXT("output-format"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("format"),	TXT("The output format (%i,%v,%t)")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

RequestCmd::RequestCmd(int argc, tchar* argv[])
	: WCL::ConsoleCmd(s_switches, s_switches+s_switchCount, argc, argv, USAGE)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

RequestCmd::~RequestCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* RequestCmd::getDescription()
{
	return TXT("Retrieve the value for one or more items");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* RequestCmd::getUsage()
{
	return TXT("USAGE: DDECmd request --server <server> --topic <topic> --item <item> ... [--format <format>]");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int RequestCmd::doExecute(tostream& out, tostream& /*err*/)
{
	// Type aliases.
	typedef std::vector<tstring> Items;
	typedef Core::CmdLineParser::StringVector::const_iterator ItemConstIter;

	tstring server;
	tstring topic;
	Items   items;

	// Validate and extract the command line arguments.
	if (m_parser.isSwitchSet(LINK))
	{
		tstring link = m_parser.getSwitchValue(LINK);
		tstring item;

		if (!CDDELink::ParseLink(link, server, topic, item))
			throw Core::InvalidArgException(Core::fmt(TXT("Invalid DDE link format '%s'"), link.c_str()));

		items.push_back(item);
	}
	else
	{
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

	const ValueFormatter formatter(valueFormat, trimValue);

	// Open the conversation.
	CDDEClient client;
	DDE::CltConvPtr conv(client.CreateConversation(server.c_str(), topic.c_str()));

	// Request the items...
	for (ItemConstIter it = items.begin(); it != items.end(); ++it)
	{
		const tstring& item = *it;

		CDDEData   ddeValue = conv->Request(item.c_str(), format);
		TextFormat stringFormat = (format != CF_UNICODETEXT) ? ANSI_TEXT : UNICODE_TEXT;
		tstring    stringValue = ddeValue.GetString(stringFormat).c_str();

		out << formatter.format(item, stringValue) << std::endl;
	}

	return EXIT_SUCCESS;
}
