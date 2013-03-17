////////////////////////////////////////////////////////////////////////////////
//! \file   ServersCmdTests.cpp
//! \brief  The tests for the ServersCmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "ServersCmd.hpp"
#include <sstream>

TEST_SET(ServersCmd)
{

TEST_CASE("The command should succeed if not arguments are specified")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("servers") };
	const int argc = ARRAY_SIZE(argv);

	ServersCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
	TEST_FALSE(out.str().empty());
}
TEST_CASE_END

}
TEST_SET_END
