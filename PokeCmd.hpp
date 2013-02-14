////////////////////////////////////////////////////////////////////////////////
//! \file   PokeCmd.hpp
//! \brief  The PokeCmd class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_POKECMD_HPP
#define APP_POKECMD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ConsoleCmd.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The DDE command used to set a value for an item.

class PokeCmd : public WCL::ConsoleCmd
{
public:
	//! Constructor.
	PokeCmd(int argc, tchar* argv[]);

	//! Destructor.
	virtual ~PokeCmd();
	
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

#endif // APP_POKECMD_HPP
