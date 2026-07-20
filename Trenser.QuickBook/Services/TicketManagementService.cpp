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
#include "PaymentManagementService.h"
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
	: m_dataStore(DataStore::getInstance()),
	  m_mutex(config::MutexMappings::TICKET_MUTEX_NAME)
{
}

/*
* Function Name : getAuthenticatedUserType
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
* Function Name : generateTicketId
* Description   : Generates a unique ticket ID based on the current number
*                 of tickets in the datastore. The ID is formatted as "USXXX".
* Parameters    : None
* Return Type   : const std::string
*/
const std::string TicketManagementService::generateTicketId()
{
	const int count = m_dataStore.getTicketCount();
	int idNumber = count + 1;
	std::ostringstream buffer;
	buffer << "TK" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

/*
* Function Name : generateTicket
* Description   : Creates a new ticket if the payment is valid and adds it
*                 to the mapped file. Returns the process status.
* Parameters    :
*                  payment   - Pointer to the Payment object associated with the ticket
*                  customer  - Pointer to the User object representing the customer
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus TicketManagementService::generateTicket(Payment* payment, User* customer)
{
	ScopedLock lock(m_mutex);
	if (!payment)
	{
		return Enums::ProcessStatus::FAILED;
	}
	Ticket* ticket = Factory::getObject<Ticket>(generateTicketId(), payment, customer);
	std::string message;
	if (ticket)
	{
		message = "New Ticket with ID : " + ticket->getTicketId() + " has created.";
		m_logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
		m_dataStore.addTicket(ticket);
		message = "Your Booking has been confirmed with Ticket ID: " + ticket->getTicketId();
		m_notificationManagementService.sendNotification(customer, message);
		return Enums::ProcessStatus::SUCCESS;
	}
	message = "Ticket generation failed.";
	m_logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : viewTicketDetails
* Description   : Retrieves all active tickets for the currently authenticated user.
*                 Only tickets with shows that are RUNNING or SCHEDULED are included.
* Parameters    : None
* Return Type   : const std::vector<Ticket*>
*/
const std::vector<const Ticket*> TicketManagementService::viewTicketDetails()
{
	ScopedLock lock(m_mutex);
	std::string authenticatedUserId = m_dataStore.getAuthenticatedUser()->getUserId();
	const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
	std::vector<const Ticket*> authenticatedUserActiveTickets;
	for (std::map<std::string, Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		if ((*iterator).second->getCustomer()->getUserId() == authenticatedUserId)
		{
			Enums::ShowStatus tikcetShowStatus = (*iterator).second->getPayment()->getBooking()->getShow()->getShowStatus();
			if ((tikcetShowStatus == Enums::ShowStatus::RUNNING || tikcetShowStatus == Enums::ShowStatus::SCHEDULED) && (*iterator).second->getTicketStatus() != Enums::TicketStatus::CANCELLED)
			{
				authenticatedUserActiveTickets.push_back((*iterator).second);
			}
		}
	}
	return authenticatedUserActiveTickets;
}

/*
* Function Name : viewAllTickets
* Description   : Returns all tickets stored in the datastore, regardless of user or status.
* Parameters    : None
* Return Type   : const std::map<std::string, Ticket*>&
*/
const std::vector<const Ticket*> TicketManagementService::viewAllTickets()
{
	ScopedLock lock(m_mutex);
	const std::map<std::string, Ticket*>& allTickets = m_dataStore.getTickets();
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
* Function Name : viewTicketHistory
* Description   : Retrieves the complete ticket history for the currently authenticated user,
*                 including both active and past tickets.
* Parameters    : None
* Return Type   : const std::vector<Ticket*>
*/
const std::vector<const Ticket*> TicketManagementService::viewTicketHistory()
{
	ScopedLock lock(m_mutex);
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

/*
* Function Name : cancelTicket
* Description   : Cancels a ticket based on the provided Ticket ID.
*                 Retrieves the ticket from the datastore, cancels the associated payment
*                 using PaymentManagementService, and returns the process status.
* Parameters    :
*                  ticketId - The unique identifier of the ticket to be cancelled
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus TicketManagementService::cancelTicket(const std::string& ticketId)
{
	ScopedLock lock(m_mutex);
	Ticket* ticket = m_dataStore.getTicketById(ticketId);
	if (!ticket)
	{
		return Enums::ProcessStatus::FAILED;
	}
	Payment* payment = ticket->getPayment();
	if (!payment)
	{
		return Enums::ProcessStatus::FAILED;
	}
	Enums::ProcessStatus status = Enums::ProcessStatus::FAILED;
	PaymentManagementService paymentManagementService;
	status = paymentManagementService.refundPayment(ticket, payment);
	std::string message;
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		m_dataStore.updateTicketStatus(ticketId, Enums::TicketStatus::CANCELLED);
		message = "Ticket with ID : " + ticket->getTicketId() + " has been cancelled.";
		m_logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
		return Enums::ProcessStatus::SUCCESS;
	}
	message = "Ticket Id " + ticket->getTicketId() + " cancellation failed";
	m_logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
	return Enums::ProcessStatus::FAILED;
}