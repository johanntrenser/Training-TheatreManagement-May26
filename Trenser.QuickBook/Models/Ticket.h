/*
 * File: Ticket.h
 * Description: Defines the Ticket class, which represents a booked ticket
 *              in the system. Stores details such as ticket ID, associated
 *              payment, and customer information.
 * Author: Entire Team
 * Created: 20-May-2026
 */

#pragma once
#include "User.h"
#include "Payment.h"
#include "ApplicationConfig.h"

class Ticket
{
private:
	std::string m_ticketId;
	Payment* m_payment;
	User* m_customer;
public: 
	Ticket();
	Ticket(const std::string& ticketId, Payment* payment, User* customer);
	const std::string& getTicketId();
	Payment* getPayment();
	User* getCustomer();
	void setTicketId(const std::string ticketId);
	void setPayment(Payment* payment);
	void setCustomer(User* customer);
	std::string serialize();
	~Ticket() = delete;
};

