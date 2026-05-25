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
#include "DataStore.h"

class TicketManagementService
{
private:
	DataStore& m_dataStore;
public:
	TicketManagementService();
	Enums::ProcessStatus generateTicket(Payment* payment, User* customer);
	const std::string generateTicketId();
	const std::vector<const Ticket*> viewTicketDetails();
	const std::vector<const Ticket*> viewAllTickets();
	const std::vector<const Ticket*> viewTicketHistory();
	Enums::UserType getAuthenticatedUserType() const;
	Enums::TicketStatus viewTicketStatus(const std::string& ticketId);
};