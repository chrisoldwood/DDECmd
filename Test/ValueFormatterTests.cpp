////////////////////////////////////////////////////////////////////////////////
//! \file   ValueFormatterTests.cpp
//! \brief  The unit tests for the ValueFormatter class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "ValueFormatter.hpp"
#include <Core/AnsiWide.hpp>

TEST_SET(ValueFormatter)
{
	const tchar* SERVER_NAME = TXT("server");
	const tchar* TOPIC_NAME = TXT("topic");
	const tchar* ITEM_NAME = TXT("item");
	const tchar* TEST_VALUE = TXT("unit test");

	const bool TRIM_VALUE = ValueFormatter::TRIM_VALUE;
	const bool NO_TRIM_VALUE = ValueFormatter::NO_TRIM_VALUE;

	const tstring DEFAULT_VALUE_FORMAT = ValueFormatter::DEFAULT_SINGLE_ITEM_FORMAT;
	const tstring DEFAULT_DATE_FORMAT = ValueFormatter::DEFAULT_DATE_FORMAT;
	const tstring DEFAULT_TIME_FORMAT = ValueFormatter::DEFAULT_TIME_FORMAT;

TEST_CASE("Formatting should format just the value by default")
{
	const tstring expected = TEST_VALUE;

	const ValueFormatter formatter(DEFAULT_VALUE_FORMAT, TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("Formatting should trim whitespace from the value by default")
{
	const tstring expected = TXT("unit test");

	const ValueFormatter formatter(DEFAULT_VALUE_FORMAT, TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TXT("  unit test  "));

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("Disabling trimming should leave whitespaace around the value")
{
	const tstring expected = TXT("  unit test  ");

	const ValueFormatter formatter(DEFAULT_VALUE_FORMAT, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TXT("  unit test  "));

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("Format string without variables should be output verbatim")
{
	const tstring format = TXT("verbatim output");
	const tstring expected = format;

	const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("Invalid format string should generate an exception")
{
	typedef const tchar** iter;

	const tchar* invalidFormats[] =
	{
		TXT("%"),
		TXT("prefix %"),
		TXT("%?"),
	};

	iter end = invalidFormats + ARRAY_SIZE(invalidFormats);

	for (iter it = invalidFormats; it != end; ++it)
	{
		const tstring format(*it);
		const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

		TEST_THROWS(formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE));
	}
}
TEST_CASE_END

TEST_CASE("%v in format string should output value")
{
	const tstring format = TXT("%v");
	const tstring expected = TXT("unit test");

	const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%s in format string should output server name")
{
	const tstring format = TXT("%s");
	const tstring expected = TXT("server");

	const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%t in format string should output topic name")
{
	const tstring format = TXT("%t");
	const tstring expected = TXT("topic");

	const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%i in format string should output item name")
{
	const tstring format = TXT("%i");
	const tstring expected = TXT("item");

	const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%% in format string should output single % char")
{
	const tstring format = TXT("%%");
	const tstring expected = TXT("%");

	const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%d in format string should output datetime in ISO format by default")
{
	const tstring format = TXT("%d");
	//const tstring expected = TXT("YYYY-MM-DDTHH:MM:SS");

	const ValueFormatter formatter(format, NO_TRIM_VALUE, DEFAULT_DATE_FORMAT, DEFAULT_TIME_FORMAT);

	const tstring actual = formatter.format(SERVER_NAME, TOPIC_NAME, ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual[ 4] == TXT('-'));
	TEST_TRUE(actual[ 7] == TXT('-'));
	TEST_TRUE(actual[10] == TXT('T'));
	TEST_TRUE(actual[13] == TXT(':'));
	TEST_TRUE(actual[16] == TXT(':'));
}
TEST_CASE_END

}
TEST_SET_END
