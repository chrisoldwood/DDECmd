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
	const tchar* ITEM_NAME = TXT("item");
	const tchar* TEST_VALUE = TXT("unit test");

	const bool TRIM_VALUE = ValueFormatter::TRIM_VALUE;
	const bool NO_TRIM_VALUE = ValueFormatter::NO_TRIM_VALUE;

	const tstring DEFAULT_FORMAT = ValueFormatter::DEFAULT_SINGLE_ITEM_FORMAT;

TEST_CASE("Formatting should format just the value by default")
{
	const tstring expected = TEST_VALUE;

	const ValueFormatter formatter(DEFAULT_FORMAT, TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("Formatting should trim whitespace from the value by default")
{
	const tstring expected = TXT("unit test");

	const ValueFormatter formatter(DEFAULT_FORMAT, TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TXT("  unit test  "));

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("Disabling trimming should leave whitespaace around the value")
{
	const tstring expected = TXT("  unit test  ");

	const ValueFormatter formatter(DEFAULT_FORMAT, NO_TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TXT("  unit test  "));

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("Format string without variables should be output verbatim")
{
	const tstring format = TXT("verbatim output");
	const tstring expected = format;

	const ValueFormatter formatter(format, NO_TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TEST_VALUE);

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
		const ValueFormatter formatter(format, NO_TRIM_VALUE);

		TEST_THROWS(formatter.format(ITEM_NAME, TEST_VALUE));
	}
}
TEST_CASE_END

TEST_CASE("%v in format string should output value")
{
	const tstring format = TXT("%v");
	const tstring expected = TXT("unit test");

	const ValueFormatter formatter(format, NO_TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%i in format string should output item name")
{
	const tstring format = TXT("%i");
	const tstring expected = TXT("item");

	const ValueFormatter formatter(format, NO_TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%% in format string should output single % char")
{
	const tstring format = TXT("%%");
	const tstring expected = TXT("%");

	const ValueFormatter formatter(format, NO_TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("%t in format string should output timestamp in ISO format by default")
{
	const tstring format = TXT("%t");
	//const tstring expected = TXT("YYYY-MM-DDTHH:MM:SS");

	const ValueFormatter formatter(format, NO_TRIM_VALUE);

	const tstring actual = formatter.format(ITEM_NAME, TEST_VALUE);

	TEST_TRUE(actual[ 4] == TXT('-'));
	TEST_TRUE(actual[ 7] == TXT('-'));
	TEST_TRUE(actual[10] == TXT('T'));
	TEST_TRUE(actual[13] == TXT(':'));
	TEST_TRUE(actual[16] == TXT(':'));
}
TEST_CASE_END

}
TEST_SET_END
