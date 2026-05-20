#include "Ticket.h"

Ticket::Ticket() :
	m_ticketId(""),
	m_payment(nullptr),
	m_customer(nullptr)
{}

Ticket::Ticket(const std::string& ticketId, Payment* payment, User* customer) : 
	m_ticketId(ticketId),
	m_payment(payment), 
	m_customer(customer)
{}

const std::string& Ticket::getTicketId()
{
	return m_ticketId;
}

Payment* Ticket::getPayment()
{
	return m_payment;
}

User* Ticket::getCustomer()
{
	return m_customer;
}

void Ticket::setTicketId(const std::string ticketId)
{
	m_ticketId = ticketId;
}

void Ticket::setPayment(Payment* payment)
{
	m_payment = payment;
}

void Ticket::setCustomer(User* customer)
{
	m_customer = customer;
}


