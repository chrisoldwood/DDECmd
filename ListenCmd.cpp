////////////////////////////////////////////////////////////////////////////////
//! \file   ListenCmd.cpp
//! \brief  The ListenCmd class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ListenCmd.hpp"
#include "CmdLineArgs.hpp"
#include <Core/CmdLineException.hpp>
#include <NCL/DDEServer.hpp>
#include "ListenSink.hpp"
#include <Core/StringUtils.hpp>
#include <NCL/DDESvrConv.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The table of command specific command line switches.

static Core::CmdLineSwitch s_switches[] =
{
	{ USAGE,	TXT("?"),	nullptr,		Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		nullptr,		TXT("Display the command syntax")	},
	{ USAGE,	nullptr,	TXT("help"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::NONE,		nullptr,		TXT("Display the command syntax")	},
	{ SERVER,	TXT("s"),	TXT("server"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("server"),	TXT("The DDE Server name")			},
	{ TOPIC,	TXT("t"),	TXT("topic"), 	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("topic"),	TXT("The DDE Server topic")			},
	{ DELAY,	nullptr,	TXT("delay"),	Core::CmdLineSwitch::ONCE,	Core::CmdLineSwitch::SINGLE,	TXT("delay"),	TXT("The delay (ms) before replying")	},
};
static size_t s_switchCount = ARRAY_SIZE(s_switches);

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

ListenCmd::ListenCmd(int argc, tchar* argv[], WCL::ConsoleApp& app)
	: WCL::ConsoleCmd(s_switches, s_switches+s_switchCount, argc, argv, USAGE)
	, m_app(app)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

ListenCmd::~ListenCmd()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the description of the command.

const tchar* ListenCmd::getDescription()
{
	return TXT("Act as a DDE server and echo interactions");
}

////////////////////////////////////////////////////////////////////////////////
//! Get the expected command usage.

const tchar* ListenCmd::getUsage()
{
	return TXT("USAGE: DDECmd listen --server <server> [--topic <topic>]");
}

////////////////////////////////////////////////////////////////////////////////
//! The implementation of the command.

int ListenCmd::doExecute(tostream& out, tostream& /*err*/)
{
	ASSERT(m_parser.getUnnamedArgs().at(0) == TXT("listen"));

	// Type alises.
	typedef CDDESvrConvs::const_iterator ConvIter;
	typedef CDDESvrLinks::const_iterator LinkIter;

	if (!m_parser.isSwitchSet(SERVER))
		throw Core::CmdLineException(TXT("No DDE server name specified [--server]"));

	tstring server = m_parser.getSwitchValue(SERVER);
	tstring topic;

	if (m_parser.isSwitchSet(TOPIC))
		topic = m_parser.getSwitchValue(TOPIC);

	DWORD delay = 0;

	if (m_parser.isSwitchSet(DELAY))
		delay = Core::parse<DWORD>(m_parser.getSwitchValue(DELAY));

	// Start listening for updates.
	CDDEServer service;
	service.Register(server.c_str());

	ListenSink::LinkValues values;
	ListenSink sink(server, topic, out, delay, values);
	service.AddListener(&sink);

	// Pump messages until the user presses Ctrl-C.
	CMsgThread& thread = m_app.mainThread();
	CEvent&     abortEvent = m_app.getAbortEvent();
	const DWORD adviseFreq = 1000;       
	uint        nextValue = 1;

	while (!abortEvent.IsSignalled())
	{
		DWORD now = ::GetTickCount();
		DWORD expireAt = now + adviseFreq;

		while(thread.ProcessMsgQueue() && (::GetTickCount() < expireAt))
			::Sleep(0);

		// Send an advise out on all links.
		CDDESvrConvs conversations;

		service.GetAllConversations(conversations);

		for (ConvIter convIt = conversations.begin(); convIt != conversations.end(); ++convIt)
		{
			CDDESvrConv* conversation = *convIt;
			CDDESvrLinks links;
			
			conversation->GetAllLinks(links);

			for (LinkIter linkIt = links.begin(); linkIt != links.end(); ++linkIt)
			{
				CDDELink* link = *linkIt;
				tstring   value = Core::format<uint>(nextValue++);

				values[link] = value;
				conversation->PostLinkUpdate(link);
			}
		}
	}

	return EXIT_SUCCESS;
}
