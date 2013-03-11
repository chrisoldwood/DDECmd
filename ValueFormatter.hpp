////////////////////////////////////////////////////////////////////////////////
//! \file   ValueFormatter.hpp
//! \brief  The ValueFormatter class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef APP_VALUEFORMATTER_HPP
#define APP_VALUEFORMATTER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The formatter used to create the final string to output for a DDE link
//! value.

class ValueFormatter
{
public:
	//! Trim whitespace around value.
	const static bool TRIM_VALUE = true;
	//! Don't trim whitespace around value.
	const static bool NO_TRIM_VALUE = false;

	//! The default single-item format.
	const static tstring DEFAULT_SINGLE_ITEM_FORMAT;
	//! The default multi-item format.
	const static tstring DEFAULT_MULTI_ITEM_FORMAT;

public:
	//! Full constructor.
	ValueFormatter(const tstring& valueFormat, bool trimValue);

	//! Destructor.
	~ValueFormatter();

	//
	// Methods.
	//

	//! Format the value using the configured style.
	tstring format(const tstring& item, const tstring& value) const;

private:
	//
	// Members.
	//
	tstring	m_valueFormat;	//!< The output format.
	bool	m_trimValue;	//!< Trim whitespace around the value.
};

#endif // APP_VALUEFORMATTER_HPP
