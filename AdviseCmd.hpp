////////////////////////////////////////////////////////////////////////////////
//! \file   AdviseCmd.hpp
//! \brief  The AdviseCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_ADVISECMD_HPP
#define APP_ADVISECMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Command.hpp"
#include <NCL/DefDDEClientListener.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The DDE command used to listen for updates the items.

class AdviseCmd : public Command,
				  public CDefDDEClientListener
{
public:
	//! Constructor.
	AdviseCmd(int argc, tchar* argv[]);

	//! Destructor.
	virtual ~AdviseCmd();
	
private:
	//
	// Command methods.
	//

	//! Get the description of the command.
	virtual const tchar* getDescription();

	//! Get the expected command usage.
	virtual const tchar* getUsage();

	//! The implementation of the command.
	virtual int doExecute();

	//
	// IDDEClientListener Methods.
	//

	//! Handle a link being updated.
	virtual void OnAdvise(CDDELink* link, const CDDEData* value);
};

#endif // APP_ADVISECMD_HPP
