#include "TicketManagementService.h"
#include <iomanip>
#include <sstream>
#include "Factory.h"

TicketManagementService::TicketManagementService()
    : m_dataStore(DataStore::getInstance())
{ }

const std::string TicketManagementService::generateTicketId()
{
    const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
    int idNumber = static_cast<int>(tickets.size()) + 1;
    std::ostringstream buffer;
    buffer << "PA" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

Enums::ProcessStatus TicketManagementService::generateTicket(Payment* payment, User* user)
{
    Ticket* ticket = Factory::getObject<Ticket>(generateTicketId(), payment, user);
    m_dataStore.addTicket(ticket);
    return Enums::ProcessStatus::SUCCESS;
}