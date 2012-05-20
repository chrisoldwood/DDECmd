////////////////////////////////////////////////////////////////////////////////
//! \file   CmdLineArgs.hpp
//! \brief  .
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_CMDLINEARGS_HPP
#define APP_CMDLINEARGS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The command line switches.

enum CmdLineArgType
{
	USAGE		= 0,	//!< Show the program options syntax.
	VERSION		= 1,	//!< Show the program version and copyright.
	SERVER		= 2,	//!< The DDE Server name.
	TOPIC		= 3,	//!< The DDE Server topic.
	ITEM		= 4,	//!< The item name.
	FORMAT		= 5,	//!< The clipboard format.
	VALUE		= 6,	//!< The value to set.
	COMMAND		= 7,	//!< The command to execute.
	MANUAL		= 8,	//!< Show the manual.
};

#endif // APP_CMDLINEARGS_HPP
