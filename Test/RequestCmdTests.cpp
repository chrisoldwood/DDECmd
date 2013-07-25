////////////////////////////////////////////////////////////////////////////////
//! \file   RequestCmdTests.cpp
//! \brief  The tests for the RequestCmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "RequestCmd.hpp"
#include <sstream>

#if __GNUC__
// deprecated conversion from string constant to 'tchar* {aka char*}'
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

TEST_SET(RequestCmd)
{

TEST_CASE("The command should display error and usage if the server name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server name specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd request")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the topic name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--server"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server topic specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd request")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the item name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No item(s) specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd request")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if a link and server/topic/item are both specified")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--link"), TXT("PROGMAN|PROGMAN!Accessories") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd request")) != nullptr);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("cannot be mixed with")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should succeed if the server, topic and item name are valid")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END

TEST_CASE("The command should fail if the clipboard format is unknown")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--format"), TXT("CF_INVALID_FORMAT") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	TEST_THROWS(command.execute(out, err));
}
TEST_CASE_END

TEST_CASE("The command should fail if the dde link is malformed")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--link"), TXT("invalid link") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	TEST_THROWS(command.execute(out, err));
}
TEST_CASE_END

TEST_CASE("The command should succeed if the dde link is valid")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--link"), TXT("PROGMAN|PROGMAN!Accessories") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END

TEST_CASE("The command should fail if the output format is malformed")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--output-format"), TXT("%?") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	TEST_THROWS(command.execute(out, err));
}
TEST_CASE_END

TEST_CASE("The command should output the date using the custom format when specified")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("-l"), TXT("PROGMAN|PROGMAN!Accessories"), TXT("-of"), TXT("%d"), TXT("--date-format"), TXT("ddd dd MMM yy") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);

//	const tstring expected = TXT("ddd dd MMM yy HH:MM:SS\n");
	const tstring output = out.str();

	TEST_TRUE(output.length() == 23);
	TEST_TRUE(output[ 3] == TXT(' '));
	TEST_TRUE(output[ 6] == TXT(' '));
	TEST_TRUE(output[10] == TXT(' '));
	TEST_TRUE(output[13] == TXT(' '));
	TEST_TRUE(output[16] == TXT(':'));
	TEST_TRUE(output[19] == TXT(':'));
	TEST_TRUE(output[22] == TXT('\n'));
}
TEST_CASE_END

TEST_CASE("The command should output the time using the custom format when specified")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("request"), TXT("-l"), TXT("PROGMAN|PROGMAN!Accessories"), TXT("-of"), TXT("%d"), TXT("--time-format"), TXT("hh mm ss tt") };
	const int argc = ARRAY_SIZE(argv);

	RequestCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);

//	const tstring expected = TXT("YYYY-MM-DD hh mm ss tt\n");
	const tstring output = out.str();

	TEST_TRUE(output.length() == 23);
	TEST_TRUE(output[ 4] == TXT('-'));
	TEST_TRUE(output[ 7] == TXT('-'));
	TEST_TRUE(output[10] == TXT(' '));
	TEST_TRUE(output[13] == TXT(' '));
	TEST_TRUE(output[16] == TXT(' '));
	TEST_TRUE(output[19] == TXT(' '));
	TEST_TRUE(output[22] == TXT('\n'));
}
TEST_CASE_END

}
TEST_SET_END
