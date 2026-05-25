/*
 * File: TicketManagementService.h
 * Description: Declares the TicketManagementService class, which provides
 *              functionality for managing tickets within the system.
 *              This service will handle operations such as issuing,
 *              validating, retrieving, and canceling tickets for shows.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <iomanip>
#include <sstream>
#include "TicketManagementService.h"
#include "Factory.h"
#include "Enums.h"

 /*
 * Function Name : default constructor
 * Description   : Constructor that initializes the TicketManagementService
 *                 by retrieving the singleton instance of DataStore.
 * Parameters    : None
 * Return Type   : None
 */
TicketManagementService::TicketManagementService()
	: m_dataStore(DataStore::getInstance())
{
}

/*
* Function Name : TicketManagementService::getAuthenticatedUserType
* Description   : Retrieves the user type of the currently authenticated user
*                 by delegating the call to the DataStore.
* Parameters    : None
* Return Type   : Enums::UserType
*/
Enums::UserType TicketManagementService::getAuthenticatedUserType() const
{
	return m_dataStore.getAuthenticatedUserType();
}

/*
* Function Name : TicketManagementService::generateTicketId
* Description   : Generates a unique ticket ID based on the current number
*                 of tickets in the datastore. The ID is formatted as "USXXX".
* Parameters    : None
* Return Type   : const std::string
*/
const std::string TicketManagementService::generateTicketId()
{
	const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
	int idNumber = static_cast<int>(tickets.size()) + 1;
	std::ostringstream buffer;
	buffer << "US" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

/*
* Function Name : TicketManagementService::generateTicket
* Description   : Creates a new ticket if the payment is valid and adds it
*                 to the datastore. Returns the process status.
* Parameters    :
*                  payment   - Pointer to the Payment object associated with the ticket
*                  customer  - Pointer to the User object representing the customer
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus TicketManagementService::generateTicket(Payment* payment, User* customer)
{
	if (!payment)
	{
		return Enums::ProcessStatus::FAILED;
	}
	Ticket* ticket = Factory::getObject<Ticket>(generateTicketId(), payment, customer);
	if (ticket)
	{
		m_dataStore.addTicket(ticket);
		Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : TicketManagementService::viewTicketDetails
* Description   : Retrieves all active tickets for the currently authenticated user.
*                 Only tickets with shows that are RUNNING or SCHEDULED are included.
* Parameters    : None
* Return Type   : const std::vector<Ticket*>
*/
const std::vector<const Ticket*> TicketManagementService::viewTicketDetails()
{
	std::string authenticatedUserId = m_dataStore.getAuthenticatedUser()->getUserId();
	const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
	std::vector<const Ticket*> authenticatedUserActiveTickets;
	for (std::map<std::string, Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		if ((*iterator).second->getCustomer()->getUserId() == authenticatedUserId)
		{
			Enums::ShowStatus tikcetShowStatus = (*iterator).second->getPayment()->getBooking()->getShow()->getShowStatus();
			if (tikcetShowStatus == Enums::ShowStatus::RUNNING || tikcetShowStatus == Enums::ShowStatus::SCHEDULED)
			{
				authenticatedUserActiveTickets.push_back((*iterator).second);
			}
		}
	}
	return authenticatedUserActiveTickets;
}

/*
* Function Name : TicketManagementService::viewAllTickets
* Description   : Returns all tickets stored in the datastore, regardless of user or status.
* Parameters    : None
* Return Type   : const std::map<std::string, Ticket*>&
*/
const std::vector<const Ticket*> TicketManagementService::viewAllTickets()
{
	std::map<std::string, Ticket*>& allTickets = m_dataStore.getTickets();
	std::vector<const Ticket*> readOnlyTickets;
	for (std::map<std::string, Ticket*>::const_iterator iterator = allTickets.begin(); iterator != allTickets.end(); ++iterator)
	{
		readOnlyTickets.push_back((*iterator).second);
	}
	if (readOnlyTickets.empty())
	{
		return {};
	}
	return readOnlyTickets;
}

/*
* Function Name : TicketManagementService::viewTicketHistory
* Description   : Retrieves the complete ticket history for the currently authenticated user,
*                 including both active and past tickets.
* Parameters    : None
* Return Type   : const std::vector<Ticket*>
*/
const std::vector<const Ticket*> TicketManagementService::viewTicketHistory()
{
	std::string authenticatedUserId = m_dataStore.getAuthenticatedUser()->getUserId();
	const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
	std::vector<const Ticket*> authenticatedUserTickets;
	for (std::map<std::string, Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		if ((*iterator).second->getCustomer()->getUserId() == authenticatedUserId)
		{
			authenticatedUserTickets.push_back((*iterator).second);
		}
	}
	return authenticatedUserTickets;
}