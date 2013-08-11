////////////////////////////////////////////////////////////////////////////////
//! \file   ExecuteCmdTests.cpp
//! \brief  The tests for the ExecuteCmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "ExecuteCmd.hpp"
#include <sstream>

#if __GNUC__
// deprecated conversion from string constant to 'tchar* {aka char*}'
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

TEST_SET(ExecuteCmd)
{

TEST_CASE("The command should display error and usage if the server name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("execute"), TXT("--topic"), TXT("PROGMAN"), TXT("--command"), TXT("anything") };
	const int argc = ARRAY_SIZE(argv);

	ExecuteCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server name specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd execute")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the topic name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("execute"), TXT("--server"), TXT("PROGMAN"), TXT("--command"), TXT("anything") };
	const int argc = ARRAY_SIZE(argv);

	ExecuteCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server topic specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd execute")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the command is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("execute"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN") };
	const int argc = ARRAY_SIZE(argv);

	ExecuteCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No command string specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd execute")) != nullptr);
}
TEST_CASE_END
/*
TEST_CASE("The command should succeed if the server, topic and command are valid")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("execute"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--command"), TXT("something") };
	const int argc = ARRAY_SIZE(argv);

	ExecuteCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END
*/
}
TEST_SET_END
