////////////////////////////////////////////////////////////////////////////////
//! \file   ListenCmd.cpp
//! \brief  The ListenCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ListenCmd.hpp"
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <NCL/DDEServer.hpp>
#include "ListenSink.hpp"
#include <Core/StringUtils.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] =
{
	{ USAGE,	TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ USAGE,	NULL,		TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ DELAY,	NULL,		TXT("delay"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("delay"),	TXT("The delay (ms) before replying")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ListenCmd::ListenCmd(int argc, tchar* argv[], WCL::ConsoleApp& app)
	: WCL::ConsoleCmd(s_switches, s_switches+s_switchCount, argc, argv, USAGE)
	, m_app(app)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ListenCmd::~ListenCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* ListenCmd::getDescription()
{
	return TXT("Act as a DDE server and echo interactions");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* ListenCmd::getUsage()
{
	return TXT("USAGE: DDECmd listen --server <server> [--topic <topic>]");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int ListenCmd::doExecute(tostream& out, tostream& /*err*/)
{
	ASSERT(m_parser.getUnnamedArgs().at(0) == TXT("listen"));

	if (!m_parser.isSwitchSet(SERVER))
		throw Core::CmdLineException(TXT("No DDE server name specified [--server]"));

	tstring server = m_parser.getSwitchValue(SERVER);
	tstring topic;

	if (m_parser.isSwitchSet(TOPIC))
		topic = m_parser.getSwitchValue(TOPIC);

	DWORD delay = 0;

	if (m_parser.isSwitchSet(DELAY))
		delay = Core::parse<uint>(m_parser.getSwitchValue(DELAY));

	// Start listening for updates.
	CDDEServer service;
	service.Register(server.c_str());

	ListenSink sink(server, topic, out, delay);
	service.AddListener(&sink);

	CMsgThread& thread = m_app.mainThread();
	CEvent&     abortEvent = m_app.getAbortEvent();

	// Pump messages until the user presses Ctrl-C.
	while (!abortEvent.IsSignalled() && thread.ProcessMsgQueue())
		thread.WaitForMessageOrSignal(abortEvent.Handle());

	return EXIT_SUCCESS;
}
