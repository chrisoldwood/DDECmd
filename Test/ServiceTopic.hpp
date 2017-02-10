////////////////////////////////////////////////////////////////////////////////
//! \file   ServiceTopic.hpp
//! \brief  The ServiceTopic class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef NCL_SERVICETOPIC_HPP
#define NCL_SERVICETOPIC_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The name of a service & topic pair.

class ServiceTopic
{
public:
	//! Constructor.
	ServiceTopic(const tstring& server, const tstring& topic);

	//
	// Members.
	//
	const tstring	m_server;	//!< The name of the DDE server.
	const tstring	m_topic;	//!< The name of a DDE topic.
};

//! The default ServiceTopic smart pointer type.
typedef Core::SharedPtr<ServiceTopic> ServiceTopicPtr;

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline ServiceTopic::ServiceTopic(const tstring& server, const tstring& topic)
	: m_server(server)
	, m_topic(topic)
{
}

#endif // NCL_SERVICETOPIC_HPP
