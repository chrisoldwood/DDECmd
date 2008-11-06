////////////////////////////////////////////////////////////////////////////////
//! \file   ServersCmd.cpp
//! \brief  The ServersCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ServersCmd.hpp"
#include <Core/tiostream.hpp>
#include <NCL/DDEClient.hpp>
#include <WCL/StrArray.hpp>
#include "CmdLineArgs.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] = 
{
	{ USAGE,	TXT("?"),	NULL,	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Display the command syntax")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ServersCmd::ServersCmd(int argc, tchar* argv[])
	: Command(s_switches, s_switches+s_switchCount, argc, argv)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ServersCmd::~ServersCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* ServersCmd::getDescription()
{
	return TXT("List the running servers and their topics");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* ServersCmd::getUsage()
{
	return TXT("USAGE: DDECmd servers");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int ServersCmd::doExecute()
{
	CDDEClient client;

	// Find the running servers.
	CStrArray servers, topics;

	client.QueryAll(servers, topics);

	// Display results.
	for (size_t i = 0, size = servers.Size(); i != size; ++i)
		tcout << servers[i] << TXT("|") << topics[i] << std::endl;

	return EXIT_SUCCESS;
}
