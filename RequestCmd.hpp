////////////////////////////////////////////////////////////////////////////////
//! \file   RequestCmd.hpp
//! \brief  The RequestCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_REQUESTCMD_HPP
#define APP_REQUESTCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Command.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The DDE command to request the value for one or more items.

class RequestCmd : public Command
{
public:
	//! Constructor.
	RequestCmd(int argc, tchar* argv[]);

	//! Destructor.
	virtual ~RequestCmd();
	
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

#endif // APP_REQUESTCMD_HPP
