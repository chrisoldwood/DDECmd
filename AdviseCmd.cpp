////////////////////////////////////////////////////////////////////////////////
//! \file   AdviseCmd.cpp
//! \brief  The AdviseCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "AdviseCmd.hpp"
#include <Core/tiostream.hpp>
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <Core/InvalidArgException.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>
#include <WCL/Clipboard.hpp>
#include <Core/StringUtils.hpp>
#include <NCL/DDEData.hpp>
#include <NCL/DDELink.hpp>
#include <WCL/ConsoleApp.hpp>
#include "DDECmd.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] = 
{
	{ USAGE,	TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ ITEM,		TXT("i"),	TXT("item"), 	Core::CmdLineSwitch::MANY,	Core::CmdLineSwitch::MULTIPLE,	TXT("item"),	TXT("The item name(s)")				},
	{ FORMAT,	TXT("f"),	TXT("format"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("format"),	TXT("The clipboard format to use")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

AdviseCmd::AdviseCmd(int argc, tchar* argv[])
	: Command(s_switches, s_switches+s_switchCount, argc, argv)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

AdviseCmd::~AdviseCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* AdviseCmd::getDescription()
{
	return TXT("Listen for updates to one or more items");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* AdviseCmd::getUsage()
{
	return TXT("USAGE: DDECmd advise --server <server> --topic <topic> --item <item> ... [--format <format>]");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int AdviseCmd::doExecute()
{
	// Type aliases.
	typedef Core::CmdLineParser::StringVector::const_iterator ItemConstIter;

	// Validate the command line arguments.
	if (!m_parser.IsSwitchSet(SERVER))
		throw Core::CmdLineException(TXT("No DDE server name specified [--server]"));

	if (!m_parser.IsSwitchSet(TOPIC))
		throw Core::CmdLineException(TXT("No DDE server topic specified [--topic]"));

	if (!m_parser.IsSwitchSet(ITEM))
		throw Core::CmdLineException(TXT("No item(s) specified [--item]"));

	tstring formatName = TXT("CF_TEXT");

	// Extract command line argument values.
	tstring server = m_parser.GetSwitchValue(SERVER);
	tstring topic  = m_parser.GetSwitchValue(TOPIC);
	const Core::CmdLineParser::StringVector& items = m_parser.GetNamedArgs().find(ITEM)->second;

	if (m_parser.IsSwitchSet(FORMAT))
		formatName = m_parser.GetSwitchValue(FORMAT);

	uint format = CClipboard::FormatHandle(formatName.c_str());

	if (format == NULL)
		throw Core::InvalidArgException(Core::Fmt(TXT("Invalid clipboard format '%s'"), formatName.c_str()));

	// Open the conversation.
	CDDEClient client;
	DDE::CltConvPtr conv(client.CreateConversation(server.c_str(), topic.c_str()));

	// Start listening for updates.
	client.AddListener(this);

	// Create the links...
	for (ItemConstIter it = items.begin(); it != items.end(); ++it)
	{
		const tstring& item = *it;

		conv->CreateLink(item.c_str(), format);
	}

	// Pump messages until the user presses Ctrl-C.
	while (!g_app.abort() && g_app.mainThread().ProcessMsgQueue())
		::WaitMessage();

	return EXIT_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//! Handle a link being updated.

void AdviseCmd::OnAdvise(CDDELink* link, const CDDEData* value)
{
	if (link->Format() != CF_UNICODETEXT)
		tcout << value->GetString(ANSI_TEXT) << std::endl;
	else
		tcout << value->GetString(UNICODE_TEXT) << std::endl;

	tcout.flush();
}
