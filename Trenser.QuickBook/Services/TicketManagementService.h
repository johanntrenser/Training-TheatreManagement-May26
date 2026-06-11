/*
 * File: TicketManagementService.h
 * Description: Declares the TicketManagementService class, which provides
 *              functionality for managing tickets within the system.
 *              This service will handle operations such as issuing,
 *              validating, retrieving, and canceling tickets for shows.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <map>
#include <string>
#include "DataStore.h"
#include "FileManagement.h"
#include "LogManagementService.h"
#include "NotificationManagementService.h"

class TicketManagementService
{
private:
	DataStore& m_dataStore;
	LogManagementService m_logManagementService;
	NotificationManagementService m_notificationManagementService;
	const std::string& PATH = config::File::TICKET_FILEPATH;
public:
	TicketManagementService();
	Enums::ProcessStatus generateTicket(Payment* payment, User* customer);
	const std::string generateTicketId();
	const std::vector<const Ticket*> viewTicketDetails();
	const std::vector<const Ticket*> viewAllTickets();
	const std::vector<const Ticket*> viewTicketHistory();
	Enums::UserType getAuthenticatedUserType() const;
	Enums::ProcessStatus cancelTicket(const std::string& ticketId);
	void saveTicketData();
	void loadTicketData();
};