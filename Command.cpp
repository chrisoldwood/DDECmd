////////////////////////////////////////////////////////////////////////////////
//! \file   Command.cpp
//! \brief  The Command class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Command.hpp"
#include <Core/CmdLineException.hpp>
#include "CmdLineArgs.hpp"
#include <Core/tiostream.hpp>

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Command::Command(SwitchCIter itFirstSwitch, SwitchCIter itLastSwitch, int argc, tchar* argv[])
	: m_argc(argc)
	, m_argv(argv)
	, m_parser(itFirstSwitch, itLastSwitch)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Command::~Command()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Execute the command.

int Command::execute(tostream& out, tostream& err)
{
	// Parse the command line.
	m_parser.parse(m_argc, m_argv);

	if (m_parser.getUnnamedArgs().size() != 1)
		throw Core::CmdLineException(TXT("Only one DDE command can be specified"));

	// Request for command help?
	if (m_parser.isSwitchSet(USAGE))
	{
		out << getDescription() << std::endl;
		out << std::endl;
		out << getUsage() << std::endl;
		out << std::endl;
		out << m_parser.formatSwitches(Core::CmdLineParser::UNIX);
		
		return EXIT_SUCCESS;
	}

	// Execute it.
	return doExecute(out, err);
}
