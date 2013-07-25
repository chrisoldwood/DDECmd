////////////////////////////////////////////////////////////////////////////////
//! \file   AdviseCmdTests.cpp
//! \brief  The tests for the AdviseCmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "AdviseCmd.hpp"
#include <sstream>

#if __GNUC__
// deprecated conversion from string constant to 'tchar* {aka char*}'
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

class AdviseCmdTestApp : public WCL::ConsoleApp
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

TEST_SET(AdviseCmd)
{
	AdviseCmdTestApp app;

TEST_CASE("The command should display error and usage if the server name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server name specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd advise")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the topic name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--server"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server topic specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd advise")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the item name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No item(s) specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd advise")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if a link and server/topic/item are both specified")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--link"), TXT("PROGMAN|PROGMAN!Accessories") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd advise")) != nullptr);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("cannot be mixed with")) != nullptr);
}
TEST_CASE_END
/*
TEST_CASE("The command should succeed if the server, topic and item name are valid")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END
*/
TEST_CASE("The command should fail if the clipboard format is unknown")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--format"), TXT("CF_INVALID_FORMAT") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	TEST_THROWS(command.execute(out, err));
}
TEST_CASE_END

TEST_CASE("The command should fail if the dde link is malformed")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--link"), TXT("invalid link") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	TEST_THROWS(command.execute(out, err));
}
TEST_CASE_END
/*
TEST_CASE("The command should succeed if the dde link is valid")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--link"), TXT("PROGMAN|PROGMAN!Accessories") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END
*/
TEST_CASE("The command should fail if the output format is malformed")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("advise"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--output-format"), TXT("%?") };
	const int argc = ARRAY_SIZE(argv);

	AdviseCmd command(argc, argv, app);

	TEST_THROWS(command.execute(out, err));
}
TEST_CASE_END

}
TEST_SET_END
