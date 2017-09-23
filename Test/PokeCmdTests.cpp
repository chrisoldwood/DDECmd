////////////////////////////////////////////////////////////////////////////////
//! \file   PokeCmdTests.cpp
//! \brief  The tests for the PokeCmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "PokeCmd.hpp"
#include <sstream>

#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) // GCC 4.2+
// deprecated conversion from string constant to 'tchar* {aka char*}'
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

TEST_SET(PokeCmd)
{

TEST_CASE("The command should display error and usage if the server name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--value"), TXT("value") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server name specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd poke")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the topic name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--server"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--value"), TXT("value") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No DDE server topic specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd poke")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the item name is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--value"), TXT("value") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No item specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd poke")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if the value is missing")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("No value specified")) != nullptr);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd poke")) != nullptr);
}
TEST_CASE_END

TEST_CASE("The command should display error and usage if a link and server/topic/item are both specified")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--link"), TXT("PROGMAN|PROGMAN!Accessories"), TXT("--value"), TXT("value") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result != EXIT_SUCCESS);
	TEST_TRUE(tstrstr(out.str().c_str(), TXT("USAGE: DDECmd poke")) != nullptr);
	TEST_TRUE(tstrstr(err.str().c_str(), TXT("cannot be mixed with")) != nullptr);
}
TEST_CASE_END
/*
TEST_CASE("The command should succeed if the server, topic, item name and value are valid")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--server"), TXT("PROGMAN"), TXT("--topic"), TXT("PROGMAN"), TXT("--item"), TXT("Accessories"), TXT("--value"), TXT("value") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END
*/
TEST_CASE("The command should fail if the dde link is malformed")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--link"), TXT("invalid link"), TXT("--value"), TXT("value") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	TEST_THROWS(command.execute(out, err));
}
TEST_CASE_END
/*
TEST_CASE("The command should succeed if the dde link is valid")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("poke"), TXT("--link"), TXT("PROGMAN|PROGMAN!Accessories") };
	const int argc = ARRAY_SIZE(argv);

	PokeCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END
*/
}
TEST_SET_END
