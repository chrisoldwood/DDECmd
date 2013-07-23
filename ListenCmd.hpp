////////////////////////////////////////////////////////////////////////////////
//! \file   ListenCmd.hpp
//! \brief  The ListenCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_LISTENCMD_HPP
#define APP_LISTENCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ConsoleCmd.hpp>
#include <WCL/ConsoleApp.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The DDE command used to run the app as a DDE server.

class ListenCmd : public WCL::ConsoleCmd
{
public:
	//! Constructor.
	ListenCmd(int argc, tchar* argv[], WCL::ConsoleApp& app);

	//! Destructor.
	virtual ~ListenCmd();
	
private:
	//
	// Members.
	//
	WCL::ConsoleApp&	m_app;	//!< The hosting application.

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

#endif // APP_LISTENCMD_HPP
