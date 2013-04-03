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
#include <WCL/ConsoleCmd.hpp>

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
	virtual int run(int argc, tchar* argv[], tistream& in, tostream& out, tostream& err);

	//! Get the name of the application.
	virtual tstring applicationName() const;

	//! Display the program options syntax.
	virtual void showUsage(tostream& out) const;

private:
	//
	// Members.
	//
	Core::CmdLineParser m_parser;		//!< The command line parser.

	//
	// Internal methods.
	//

	//! Create the Comand object.
	WCL::ConsoleCmdPtr createCommand(int argc, tchar* argv[]); // throw(CmdLineException)
};

#endif // DDECMD_HPP
