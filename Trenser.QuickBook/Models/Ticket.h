#pragma once
#include "User.h"
#include "Payment.h"

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
	~Ticket() = delete;
};

