////////////////////////////////////////////////////////////////////////////////
//! \file   DDECmd.cpp
//! \brief  The DDECmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "DDECmd.hpp"
#include <WCL/Path.hpp>
#include <WCL/VerInfoReader.hpp>
#include <Core/CmdLineException.hpp>
#include <Core/BadLogicException.hpp>
#include <Core/StringUtils.hpp>
#include "ServersCmd.hpp"
#include "RequestCmd.hpp"
#include "AdviseCmd.hpp"
#include "PokeCmd.hpp"
#include "ExecuteCmd.hpp"
#include "CmdLineArgs.hpp"
#include <ostream>

////////////////////////////////////////////////////////////////////////////////
// Global variables.

//! The application object.
DDECmd g_app;

////////////////////////////////////////////////////////////////////////////////
//! The table of command line switches.

static Core::CmdLineSwitch s_switches[] =
{
	{ USAGE,	TXT("?"),	NULL,			Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Display the program options syntax")	},
	{ USAGE,	TXT("h"),	TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Display the program options syntax")	},
	{ VERSION,	TXT("v"),	TXT("version"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Display the program version")			},
	{ MANUAL,	NULL,		TXT("manual"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,	NULL,	TXT("Display the manual")					},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

DDECmd::DDECmd()
	: m_parser(s_switches, s_switches+s_switchCount)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

DDECmd::~DDECmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Run the application.

int DDECmd::run(int argc, tchar* argv[], tistream& /*in*/, tostream& out, tostream& err)
{
	// Command specified?
	if ( (argc > 1) && ((argv[1][0] != TXT('/')) && (argv[1][0] != TXT('-'))) )
	{
		// Get command and execute.
		WCL::ConsoleCmdPtr command = createCommand(argc, argv);

		return command->execute(out, err);
	}

	m_parser.parse(argc, argv, Core::CmdLineParser::ALLOW_ANY_FORMAT);

	// Request for command line syntax?
	if (m_parser.isSwitchSet(USAGE))
	{
		showUsage(out);
		return EXIT_SUCCESS;
	}
	// Request for version?
	else if (m_parser.isSwitchSet(VERSION))
	{
		showVersion(out);
		return EXIT_SUCCESS;
	}
	// Request for the manual?
	else if (m_parser.isSwitchSet(MANUAL))
	{
		showManual(err);
		return EXIT_SUCCESS;
	}

	throw Core::CmdLineException(TXT("No DDE command specified"));
}

////////////////////////////////////////////////////////////////////////////////
//! Create the Comand object.

WCL::ConsoleCmdPtr DDECmd::createCommand(int argc, tchar* argv[])
{
	ASSERT(argc > 1);

	// Validate command line.
	const tchar* command = argv[1];

	// Create command.
	if (tstricmp(command, TXT("servers")) == 0)
	{
		return WCL::ConsoleCmdPtr(new ServersCmd(argc, argv));
	}
	else if (tstricmp(command, TXT("request")) == 0)
	{
		return WCL::ConsoleCmdPtr(new RequestCmd(argc, argv));
	}
	else if (tstricmp(command, TXT("advise")) == 0)
	{
		return WCL::ConsoleCmdPtr(new AdviseCmd(argc, argv));
	}
	else if (tstricmp(command, TXT("poke")) == 0)
	{
		return WCL::ConsoleCmdPtr(new PokeCmd(argc, argv));
	}
	else if (tstricmp(command, TXT("execute")) == 0)
	{
		return WCL::ConsoleCmdPtr(new ExecuteCmd(argc, argv));
	}

	throw Core::CmdLineException(Core::fmt(TXT("Unknown DDE command: '%s'"), command));
}

////////////////////////////////////////////////////////////////////////////////
//! Get the name of the application.

tstring DDECmd::applicationName() const
{
	return TXT("DDECmd");
}

////////////////////////////////////////////////////////////////////////////////
//! Display the program options syntax.

void DDECmd::showUsage(tostream& out) const
{
	out << std::endl;
	out << TXT("USAGE: ") << applicationName() << (" <command> [options] ...") << std::endl;
	out << std::endl;

	size_t width = 16;

	out << TXT("where <command> is one of:-") << std::endl;
	out << std::endl;
	out << TXT("servers") << tstring(width-7, TXT(' ')) << ("List the running servers and their topics") << std::endl;
	out << TXT("request") << tstring(width-7, TXT(' ')) << ("Retrieve the value for one or more items") << std::endl;
	out << TXT("advise")  << tstring(width-6, TXT(' ')) << ("Listen for updates to one or more items") << std::endl;
	out << TXT("poke")    << tstring(width-4, TXT(' ')) << ("Set the value for a single item") << std::endl;
	out << TXT("execute") << tstring(width-7, TXT(' ')) << ("Send a command for execution") << std::endl;
	out << std::endl;

	out << TXT("For help on an individual command use:-") << std::endl;
	out << std::endl;
	out << TXT("DDECmd <command> -?") << std::endl;
	out << std::endl;

	out << TXT("Non-command options:-") << std::endl;
	out << std::endl;
	out << m_parser.formatSwitches(Core::CmdLineParser::UNIX);
}
