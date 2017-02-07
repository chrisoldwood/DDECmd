////////////////////////////////////////////////////////////////////////////////
//! \file   ServersCmdTests.cpp
//! \brief  The tests for the ServersCmd class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include "ServersCmd.hpp"
#include <sstream>
#include <NCL/DDEClientFactory.hpp>
#include "MockDDEClient.hpp"

#if __GNUC__
// deprecated conversion from string constant to 'tchar* {aka char*}'
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

static Core::SharedPtr<MockDDEClient> mockClient(new MockDDEClient);

static DDE::IDDEClientPtr createClient(DWORD /*flags*/)
{
	return mockClient;
}

TEST_SET(ServersCmd)
{
	DDE::DDEClientFactory::registerFactory(createClient);

TEST_CASE("The command requires no arguments by default")
{
	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("servers") };
	const int argc = ARRAY_SIZE(argv);

	ServersCmd command(argc, argv);

	int result = command.execute(out, err);

	TEST_TRUE(result == EXIT_SUCCESS);
}
TEST_CASE_END

TEST_CASE("The command lists all running servers")
{
	const tstring server(TXT("test_server"));
	const tstring topic(TXT("test_topic"));

	tostringstream out, err;

	tchar*    argv[] = { TXT("Test.exe"), TXT("servers") };
	const int argc = ARRAY_SIZE(argv);

	ServersCmd command(argc, argv);

	mockClient->m_servers.clear();
	mockClient->m_servers.push_back(server);
	mockClient->m_topics.clear();
	mockClient->m_topics.push_back(topic);

	command.execute(out, err);

	tstring output = out.str();

	TEST_TRUE(!output.empty());
	TEST_TRUE(output.find(server) != tstring::npos)
	TEST_TRUE(output.find(topic) != tstring::npos)
}
TEST_CASE_END

}
TEST_SET_END
