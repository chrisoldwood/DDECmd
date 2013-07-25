////////////////////////////////////////////////////////////////////////////////
//! \file   ListenCmdTests.cpp
//! \brief  The tests for the ListenCmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "ListenCmd.hpp"
#include <sstream>

#if __GNUC__
// deprecated conversion from string constant to 'tchar* {aka char*}'
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

class ListenCmdTestApp : public WCL::ConsoleApp
{
	virtual int run(int /*nArgc*/, tchar* /*apszArgv*/[], tistream& /*in*/, tostream& /*out*/, tostream& /*err*/)
	{
		return -1;
	}

	virtual tstring applicationName() const
	{
		return TXT("test");
	}

	virtual void showUsage(tostream& /*out*/) const
	{
	}
};

TEST_SET(ListenCmd)
{
	ListenCmdTestApp app;

TEST_CASE("The command should display error and usage if the server name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("listen"), TXT("--topic"), TXT("TopicName") };
	const int argc = ARRAY_SIZE(argv);

	ListenCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server name specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd listen")) != nullptr);
}
TEST_CASE_END
/*
TEST_CASE("The command should succeed if only the server name is supplied")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("listen"), TXT("--server"), TXT("ServerName") };
	const int argc = ARRAY_SIZE(argv);

	ListenCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END
*//*
TEST_CASE("The command should succeed if the server and topic name are supplied")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("listen"), TXT("--server"), TXT("ServerName"), TXT("--topic"), TXT("TopicName") };
	const int argc = ARRAY_SIZE(argv);

	ListenCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END
*/
}
TEST_SET_END
