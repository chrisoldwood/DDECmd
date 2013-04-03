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
	LINK		= 9,	//!< The DDE link.
	NO_TRIM		= 10,	//!< Don't trim whitespace around the value.
	OUT_FMT		= 11,	//!< The output format.
	DATE_FMT	= 12,	//!< The date format for the timestamp.
	TIME_FMT	= 13,	//!< The time format for the timestamp.
};

#endif // APP_CMDLINEARGS_HPP
