////////////////////////////////////////////////////////////////////////////////
//! \file   FetchCmd.hpp
//! \brief  The FetchCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_FETCHCMD_HPP
#define APP_FETCHCMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ConsoleCmd.hpp>
#include <WCL/ConsoleApp.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The DDE command used to fetch item values using and advise + request.

class FetchCmd : public WCL::ConsoleCmd
{
public:
	//! Constructor.
	FetchCmd(int argc, tchar* argv[], WCL::ConsoleApp& app);

	//! Destructor.
	virtual ~FetchCmd();
	
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

#endif // APP_FETCHCMD_HPP
