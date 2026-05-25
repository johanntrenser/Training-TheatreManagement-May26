#include <iomanip>
#include <sstream>
#include "TicketManagementService.h"
#include "Factory.h"
#include "Enums.h"

TicketManagementService::TicketManagementService()
	: m_dataStore(DataStore::getInstance())
{
}

const std::string TicketManagementService::generateTicketId()
{
	const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
	int idNumber = static_cast<int>(tickets.size()) + 1;
	std::ostringstream buffer;
	buffer << "US" << std::setw(3) << std::setfill('0') << idNumber;
	return buffer.str();
}

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