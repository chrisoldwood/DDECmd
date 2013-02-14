////////////////////////////////////////////////////////////////////////////////
//! \file   ExecuteCmd.hpp
//! \brief  The ExecuteCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_EXECUTECMD_HPP
#define APP_EXECUTECMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ConsoleCmd.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The DDE command used to send a string for execution by the server.

class ExecuteCmd : public WCL::ConsoleCmd
{
public:
	//! Constructor.
	ExecuteCmd(int argc, tchar* argv[]);

	//! Destructor.
	virtual ~ExecuteCmd();
	
private:
	//
	// Command methods.
	//

	//! Get the description of the command.
	virtual const tchar* getDescription();

	//! Get the expected command usage.
	virtual const tchar* getUsage();

	//! The implementation of the command.
	virtual int doExecute(tostream& out, tostream& err);
};

#endif // APP_EXECUTECMD_HPP
