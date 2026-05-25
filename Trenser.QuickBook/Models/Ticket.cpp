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
	m_customer(nullptr)
{}

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
	m_customer(customer)
{}

/*
 * Function: Ticket::getTicketId
 * Description: Retrieves the unique ticket ID.
 * Returns:
 *    const std::string& - Ticket ID
 */
const std::string& Ticket::getTicketId()
{
	return m_ticketId;
}

/*
 * Function: Ticket::getPayment
 * Description: Retrieves the associated payment object.
 * Returns:
 *    Payment* - Pointer to the payment
 */
Payment* Ticket::getPayment()
{
	return m_payment;
}

/*
 * Function: Ticket::getCustomer
 * Description: Retrieves the customer who owns the ticket.
 * Returns:
 *    User* - Pointer to the customer
 */
User* Ticket::getCustomer()
{
	return m_customer;
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