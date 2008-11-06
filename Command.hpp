////////////////////////////////////////////////////////////////////////////////
//! \file   Command.hpp
//! \brief  The Command class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_COMMAND_HPP
#define APP_COMMAND_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <Core/CmdLineParser.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The base class for all commands.

class Command
{
public:
	//! Destructor.
	virtual ~Command();

	//
	// Methods.
	//

	//! Execute the command.
	virtual int execute();

protected:
	//! Type aliases.
	typedef Core::CmdLineParser::SwitchCIter SwitchCIter;

	//! Default constructor.
	Command(SwitchCIter itFirstSwitch, SwitchCIter itLastSwitch, int argc, tchar* argv[]);

	//
	// Members.
	//
	int					m_argc;		//!< The number of command line arguments.
	tchar**				m_argv;		//!< The array of command line arguments
	Core::CmdLineParser m_parser;	//!< The command specifc command line parser.

	//
	// Callback methods.
	//

	//! Get the description of the command.
	virtual const tchar* getDescription() = 0;

	//! Get the expected command usage.
	virtual const tchar* getUsage() = 0;

	//! The implementation of the command.
	virtual int doExecute() = 0;
};

//! The default Command smart-pointer type.
typedef Core::SharedPtr<Command> CommandPtr;

#endif // APP_COMMAND_HPP
