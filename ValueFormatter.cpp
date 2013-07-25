////////////////////////////////////////////////////////////////////////////////
//! \file   ValueFormatter.cpp
//! \brief  The ValueFormatter class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ValueFormatter.hpp"
#include <Core/StringUtils.hpp>
#include <Core/ParseException.hpp>
#include <WCL/DateTime.hpp>
#include <WCL/Win32Exception.hpp>
#include <malloc.h>

#if __GNUC__
// missing initializer for member 'X'
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants.

//! The default single-item format.
const tstring ValueFormatter::DEFAULT_SINGLE_ITEM_FORMAT = TXT("%v");
//! The default multi-item format.
const tstring ValueFormatter::DEFAULT_MULTI_ITEM_FORMAT = TXT("%i: %v");

//! The default timestamp date format.
const tstring ValueFormatter::DEFAULT_DATE_FORMAT = TXT("yyyy'-'MM'-'dd");
//! The default timestamp time format.
const tstring ValueFormatter::DEFAULT_TIME_FORMAT = TXT("hh':'mm':'ss");

////////////////////////////////////////////////////////////////////////////////
//! Full constructor.

ValueFormatter::ValueFormatter(const tstring& valueFormat,
							   bool           trimValue,
							   const tstring& dateFormat,
							   const tstring& timeFormat)
	: m_valueFormat(valueFormat)
	, m_trimValue(trimValue)
	, m_dateFormat(dateFormat)
	, m_timeFormat(timeFormat)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ValueFormatter::~ValueFormatter()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Format the value using the configured style.

static tstring formatTimestamp(const tstring& dateFormat, const tstring& timeFormat)
{
	SYSTEMTIME now = { 0 };

	::GetLocalTime(&now);

	tstring date;

	if (!dateFormat.empty())
	{
		size_t length = ::GetDateFormat(LOCALE_USER_DEFAULT, 0, &now, dateFormat.c_str(), nullptr, 0);
		tchar* buffer = static_cast<tchar*>(alloca(Core::numBytes<tchar>(length)));

		int result = ::GetDateFormat(LOCALE_USER_DEFAULT, 0, &now, dateFormat.c_str(), buffer, static_cast<int>(length));

		if (result == 0)
			throw new WCL::Win32Exception(TXT("Failed to format date"));

		date = buffer;
	}

	tstring time;

	if (!timeFormat.empty())
	{
		size_t length = ::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &now, timeFormat.c_str(), nullptr, 0);
		tchar* buffer = static_cast<tchar*>(alloca(Core::numBytes<tchar>(length)));

		int result = ::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &now, timeFormat.c_str(), buffer, static_cast<int>(length));

		if (result == 0)
			throw new WCL::Win32Exception(TXT("Failed to format time"));

		time = buffer;
	}

	if (date.empty())
		return time;

	if (time.empty())
		return date;

	return date + TXT(" ") + time;
}

////////////////////////////////////////////////////////////////////////////////
//! Format the value using the configured style.

tstring ValueFormatter::format(const tstring& server,
							   const tstring& topic,
							   const tstring& item,
							   const tstring& value) const
{
	typedef tstring::const_iterator citer;

	tstring result;

	result.reserve(m_valueFormat.length() + item.length() + value.length());

	for (citer it = m_valueFormat.begin(); it != m_valueFormat.end(); ++it)
	{
		if (*it == TXT('%'))
		{
			citer variableIt = it+1;

			if (variableIt == m_valueFormat.end())
				throw Core::ParseException(Core::fmt(TXT("Invalid output format: '%s'"), m_valueFormat.c_str()));

			const tchar variable = *variableIt;

			if (variable == TXT('%'))
			{
				result += TXT('%');
			}
			else if (variable == TXT('v'))
			{
				if (m_trimValue)
					result += Core::trimCopy(value);
				else
					result += value;
			}
			else if (variable == TXT('s'))
			{
				result += server;
			}
			else if (variable == TXT('t'))
			{
				result += topic;
			}
			else if (variable == TXT('i'))
			{
				result += item;
			}
			else if (variable == TXT('d'))
			{
				result += formatTimestamp(m_dateFormat, m_timeFormat);
			}
			else
			{
				throw Core::ParseException(Core::fmt(TXT("Invalid output variable: '%%%c'"), variable));
			}

			++it;
		}
		else
		{
			result += *it;
		}
	}

	return result;
}
