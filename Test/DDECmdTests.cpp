////////////////////////////////////////////////////////////////////////////////
//! \file   DDECmdTests.cpp
//! \brief  The tests for the DDECmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "DDECmd.hpp"
#include <sstream>

TEST_SET(DDECmd)
{
/*
TEST_CASE("The app should display an error and usage when an unknown option is used")
{
	tistringstream in;
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("servers"), TXT("--invalid-switch") };
	const int argc = ARRAY_SIZE(argv);

	DDECmd app;

	int result = app.main(argc, argv, in, out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_FALSE(err.str().empty());
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd servers")) != nullptr);
}
TEST_CASE_END
*/
}
TEST_SET_END
