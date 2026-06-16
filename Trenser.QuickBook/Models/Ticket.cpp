/*
 * File: Ticket.cpp
 * Description: Implements the Ticket class, which represents a movie ticket.
 *              Stores details such as ticket ID, associated payment, and customer.
 * Author: Entire Team
 * Created: 20-May-2026
 */
#include "Ticket.h"

 /*
  * Function: Ticket::Ticket
  * Description: Default constructor initializing ticket with default values.
  * Returns:
  *    Ticket object
  */
Ticket::Ticket() :
	m_ticketId(""),
	m_payment(nullptr),
	m_customer(nullptr),
	m_status(Enums::TicketStatus::ACTIVE)
{
}

/*
 * Function: Ticket::Ticket
 * Description: Parameterized constructor initializing ticket details.
 * Parameters:
 *    const std::string& ticketId - Unique ticket identifier
 *    Payment* payment - Associated payment object
 *    User* customer - Customer who owns the ticket
 * Returns:
 *    Ticket object
 */
Ticket::Ticket(const std::string& ticketId, Payment* payment, User* customer) :
	m_ticketId(ticketId),
	m_payment(payment),
	m_customer(customer),
	m_status(Enums::TicketStatus::ACTIVE)
{
}

/*
 * Function: Ticket::getTicketId
 * Description: Retrieves the unique ticket ID.
 * Returns:
 *    const std::string& - Ticket ID
 */
const std::string& Ticket::getTicketId() const
{
	return m_ticketId;
}

/*
 * Function: Ticket::getPayment
 * Description: Retrieves the associated payment object.
 * Returns:
 *    Payment* - Pointer to the payment
 */
Payment* Ticket::getPayment() const
{
	return m_payment;
}

/*
 * Function: Ticket::getCustomer
 * Description: Retrieves the customer who owns the ticket.
 * Returns:
 *    User* - Pointer to the customer
 */
User* Ticket::getCustomer() const
{
	return m_customer;
}

/*
 * Function: getTicketStatus
 * Description: Retrieves the ticket status.
 * Returns:
 *    enum - TicketStatus
 */
Enums::TicketStatus Ticket::getTicketStatus() const
{
	return m_status;
}

/*
 * Function: Ticket::setTicketId
 * Description: Sets the unique ticket ID.
 * Parameters:
 *    const std::string ticketId - New ticket ID
 * Returns:
 *    void
 */
void Ticket::setTicketId(const std::string ticketId)
{
	m_ticketId = ticketId;
}

/*
 * Function: Ticket::setPayment
 * Description: Sets the associated payment object.
 * Parameters:
 *    Payment* payment - Pointer to the payment
 * Returns:
 *    void
 */
void Ticket::setPayment(Payment* payment)
{
	m_payment = payment;
}

/*
 * Function: Ticket::setCustomer
 * Description: Sets the customer who owns the ticket.
 * Parameters:
 *    User* customer - Pointer to the customer
 * Returns:
 *    void
 */
void Ticket::setCustomer(User* customer)
{
	m_customer = customer;
}

/*
 * Function: setTicketStatus
 * Description: Sets the ticket status.
 * Parameters:
 *    enum - ticket status
 * Returns:
 *    void
 */
void Ticket::setTicketStatus(Enums::TicketStatus status)
{
	m_status = status;
}

/*
 * Function: serialize
 * Description: Converts a Ticket object into a SharedTicket struct suitable
 *              for storage in a memory-mapped file. Copies ticket ID, payment ID,
 *              customer ID, and status into fixed-size character arrays.
 * Parameters:
 *    sharedTicket - Reference to a SharedTicket struct to populate with serialized data
 * Returns:
 *    None
 */
void Ticket::serialize(SharedTicket& sharedTicket) const
{
	sharedTicket = {};
	strncpy_s(sharedTicket.ticketId, m_ticketId.c_str(), sizeof(sharedTicket.ticketId));
	strncpy_s(sharedTicket.paymentId, 
		(m_payment ? m_payment->getPaymentId().c_str() : ""), 
		sizeof(sharedTicket.paymentId));
	strncpy_s(sharedTicket.customerId, 
		(m_customer ? m_customer->getUserId().c_str() : ""), 
		sizeof(sharedTicket.customerId));
	sharedTicket.status = static_cast<int>(m_status);
}

/*
 * Function: deserialize
 * Description: Reconstructs a Ticket object from a SharedTicket struct.
 *              Initializes ticket attributes and sets its status.
 * Parameters:
 *    sharedTicket - Pointer to a SharedTicket struct containing serialized ticket data
 * Returns:
 *    Pointer to a newly created Ticket object, or nullptr if input is invalid
 */
Ticket* Ticket::deserialize(const SharedTicket* sharedTicket)
{
	if (sharedTicket == nullptr)
	{
		return nullptr;
	}
	Ticket* ticket = Factory::getObject<Ticket>(sharedTicket->ticketId, nullptr, nullptr);
	if (ticket != nullptr)
	{
		ticket->setTicketStatus(static_cast<Enums::TicketStatus>(sharedTicket->status));
	}
	return ticket;
}