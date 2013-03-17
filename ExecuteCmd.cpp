////////////////////////////////////////////////////////////////////////////////
//! \file   ExecuteCmd.cpp
//! \brief  The ExecuteCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ExecuteCmd.hpp"
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <NCL/DDEClient.hpp>
#include <NCL/DDECltConvPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] = 
{
	{ USAGE,	TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ USAGE,	NULL,		TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		NULL,			TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ COMMAND,	TXT("c"),	TXT("command"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("command"),	TXT("The command to execute")		},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ExecuteCmd::ExecuteCmd(int argc, tchar* argv[])
	: WCL::ConsoleCmd(s_switches, s_switches+s_switchCount, argc, argv, USAGE)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ExecuteCmd::~ExecuteCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* ExecuteCmd::getDescription()
{
	return TXT("Send a command for execution");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* ExecuteCmd::getUsage()
{
	return TXT("USAGE: DDECmd execute --server <server> --topic <topic> --command <command>");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int ExecuteCmd::doExecute(tostream& /*out*/, tostream& /*err*/)
{
	ASSERT(m_parser.getUnnamedArgs().at(0) == TXT("execute"));

	// Validate the command line arguments.
	if (!m_parser.isSwitchSet(SERVER))
		throw Core::CmdLineException(TXT("No DDE server name specified [--server]"));

	if (!m_parser.isSwitchSet(TOPIC))
		throw Core::CmdLineException(TXT("No DDE server topic specified [--topic]"));

	if (!m_parser.isSwitchSet(COMMAND))
		throw Core::CmdLineException(TXT("No command string specified [--command]"));

	// Extract command line argument values.
	tstring server  = m_parser.getSwitchValue(SERVER);
	tstring topic   = m_parser.getSwitchValue(TOPIC);
	tstring command = m_parser.getSwitchValue(COMMAND);

	// Open the conversation and send the command.
	CDDEClient client;
	DDE::CltConvPtr conv(client.CreateConversation(server.c_str(), topic.c_str()));

	conv->Execute(command.c_str());

	return EXIT_SUCCESS;
}
