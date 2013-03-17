////////////////////////////////////////////////////////////////////////////////
//! \file   ValueFormatter.cpp
//! \brief  The ValueFormatter class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ValueFormatter.hpp"
#include <Core/StringUtils.hpp>
#include <Core/ParseException.hpp>
#include <WCL/DateTime.hpp>

////////////////////////////////////////////////////////////////////////////////
// Constants.

//! The default single-item format.
const tstring ValueFormatter::DEFAULT_SINGLE_ITEM_FORMAT = TXT("%v");
//! The default multi-item format.
const tstring ValueFormatter::DEFAULT_MULTI_ITEM_FORMAT = TXT("%i: %v");

//! The default timestamp date format.
const tstring ValueFormatter::DEFAULT_DATE_FORMAT = TXT("date");
//! The default timestamp time format.
const tstring ValueFormatter::DEFAULT_TIME_FORMAT = TXT("time");

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

static tstring formatTimestamp(const tstring& /*dateFormat*/, const tstring& /*timeFormat*/)
{
	return CDateTime::Current().ToString(CDate::FMT_ISO, CTime::FMT_ISO).c_str();
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
