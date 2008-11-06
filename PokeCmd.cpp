////////////////////////////////////////////////////////////////////////////////
//! \file   PokeCmd.cpp
//! \brief  The PokeCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "PokeCmd.hpp"
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <Core/AnsiWide.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include <NCL/DDEData.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] = 
{
	{ USAGE,	TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ ITEM,		TXT("i"),	TXT("item"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("item"),	TXT("The item name(s)")				},
	{ VALUE,	TXT("v"),	TXT("value"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("value"),	TXT("The value to poke")			},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

PokeCmd::PokeCmd(int argc, tchar* argv[])
	: Command(s_switches, s_switches+s_switchCount, argc, argv)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

PokeCmd::~PokeCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* PokeCmd::getDescription()
{
	return TXT("Set the value for a single item");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* PokeCmd::getUsage()
{
	return TXT("USAGE: DDECmd poke --server <server> --topic <topic> --item <item> --value <value>");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int PokeCmd::doExecute()
{
	// Validate the command line arguments.
	if (!m_parser.IsSwitchSet(SERVER))
		throw Core::CmdLineException(TXT("No DDE server name specified [--server]"));

	if (!m_parser.IsSwitchSet(TOPIC))
		throw Core::CmdLineException(TXT("No DDE server topic specified [--topic]"));

	if (!m_parser.IsSwitchSet(ITEM))
		throw Core::CmdLineException(TXT("No item specified [--item]"));

	if (!m_parser.IsSwitchSet(VALUE))
		throw Core::CmdLineException(TXT("No value specified [--value]"));

	// Extract command line argument values.
	tstring server = m_parser.GetSwitchValue(SERVER);
	tstring topic  = m_parser.GetSwitchValue(TOPIC);
	tstring item   = m_parser.GetSwitchValue(ITEM);
	tstring value  = m_parser.GetSwitchValue(VALUE);

	// Open the conversation.
	CDDEClient client;
	DDE::CltConvPtr conv(client.CreateConversation(server.c_str(), topic.c_str()));

	// Set the value.
	std::string ansiValue(T2A(value.c_str()));
	CDDEData    ddevalue(

	conv->

	for (ItemConstIter it = items.begin(); it != items.end(); ++it)
	{
		const tstring& item = *it;

		CDDEData value = conv->Request(item.c_str(), format);
		
		if (format != CF_UNICODETEXT)
			tcout << value.GetString(ANSI_TEXT) << std::endl;
		else
			tcout << value.GetString(UNICODE_TEXT) << std::endl;
	}

	return EXIT_SUCCESS;
}
