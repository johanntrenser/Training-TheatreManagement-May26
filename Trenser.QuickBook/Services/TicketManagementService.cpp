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