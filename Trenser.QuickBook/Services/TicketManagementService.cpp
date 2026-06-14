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
	buffer << "TK" << std::setw(3) << std::setfill('0') << idNumber;
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
	std::string message = "New Ticket with ID : " + ticket->getTicketId() + " has created.";
	m_logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
	if (ticket)
	{
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
			if ((tikcetShowStatus == Enums::ShowStatus::RUNNING || tikcetShowStatus == Enums::ShowStatus::SCHEDULED) && (*iterator).second->getTicketStatus() != Enums::TicketStatus::CANCELLED)
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
	Ticket* ticket = m_dataStore.getTicketById(ticketId);
	if (!ticket)
	{
		return Enums::ProcessStatus::FAILED;
	}
	Payment* payment = ticket->getPayment();
	Enums::ProcessStatus status = Enums::ProcessStatus::FAILED;
	PaymentManagementService paymentManagementService;
	status = paymentManagementService.refundPayment(ticket, payment);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		std::string message = "Ticket with ID : " + ticket->getTicketId() + " has been cancelled.";
		m_logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
		return Enums::ProcessStatus::SUCCESS;
	}
	std::string message = "Ticket Id " + ticket->getTicketId() + " cancellation failed";
	m_logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: TicketManagementService::saveTicketData
 * Description: Saves all ticket data from the DataStore into a CSV file.
 *              Uses a configurable header (from config::Header::TICKET_HEADER)
 *              and delegates serialization of each Ticket object to its
 *              serialize() method for consistent formatting.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void TicketManagementService::saveTicketData()
{
	std::vector<std::string> lines;
	lines.push_back(config::Header::TICKET_HEADER);
	const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
	for (std::map<std::string, Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		lines.push_back((iterator->second)->serialize());
	}
	FileManagement::writeLines(std::string(config::File::TICKET_FILEPATH), lines);
}

/*
 * Function: TicketManagementService::loadTicketData
 * Description: Loads all ticket data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Ticket object via Ticket::deserialize,
 *              and restores associations with Payment and Customer objects
 *              if their IDs are present and found in the DataStore.
 *              Finally, adds the reconstructed Ticket to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void TicketManagementService::loadTicketData()
{
	std::string ticketId;
	std::string paymentId;
	std::string customerId;
	std::string status;
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	for (int index = 1; index < lines.size(); index++)
	{
		Ticket* ticket = Ticket::deserialize(lines[index]);
		std::stringstream lineStream(lines[index]);
		std::getline(lineStream, ticketId, ',');
		std::getline(lineStream, paymentId, ',');
		std::getline(lineStream, customerId, ',');
		std::getline(lineStream, status, ',');
		if (!paymentId.empty())
		{
			const std::map<std::string, Payment*>& payments = m_dataStore.getPayments();
			std::map<std::string, Payment*>::const_iterator iterator = payments.find(paymentId);
			if (iterator != payments.end())
			{
				ticket->setPayment(iterator->second);
			}
		}
		if (!customerId.empty())
		{
			User* customer = m_dataStore.getUserById(customerId);
			ticket->setCustomer(customer);
		}
		ticket->setTicketStatus(Enums::getTicketStatus(status));
		m_dataStore.addTicket(ticket);
	}
}