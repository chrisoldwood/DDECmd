////////////////////////////////////////////////////////////////////////////////
//! \file   DDECmd.hpp
//! \brief  The DDECmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef DDECMD_HPP
#define DDECMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ConsoleApp.hpp>
#include "Command.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The application.

class DDECmd : public WCL::ConsoleApp
{
public:
	//! Default constructor.
	DDECmd();

	//! Destructor.
	virtual ~DDECmd();
	
protected:
	//
	// ConsoleApp methods.
	//

	//! Run the application.
	virtual int run(int argc, tchar* argv[]);

	//! Display the program options syntax.
	virtual void showUsage();

private:
	//
	// Members.
	//
	Core::CmdLineParser m_parser;		//!< The command line parser.

	//
	// Internal methods.
	//

	//! Create the Comand object.
	CommandPtr createCommand(int argc, tchar* argv[]); // throw(CmdLineException)

	//! Display the program version.
	void showVersion();
};

#endif // DDECMD_HPP
