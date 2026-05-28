#include "TicketManagementService.h"

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
	std::string paymentId, customerId;
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	for (int index = 0; index < lines.size(); index++)
	{
		Ticket* ticket = Ticket::deserialize(lines[index]);
		std::stringstream lineStream(lines[index]);
		std::getline(lineStream, paymentId, ',');
		std::getline(lineStream, customerId, ',');
		if (!paymentId.empty())
		{
			const std::map < std::string, Payment*> payment = m_dataStore.getPayments();
			if (payment.find(paymentId) != payment.end())
			{
				ticket->setPayment(payment.at(paymentId));
			}
		}
		if (!customerId.empty())
		{
			const std::map<std::string, User*> customer = m_dataStore.getUsers();
			if (customer.find(customerId) != customer.end())
			{
				ticket->setCustomer(customer.at(customerId));
			}
		}
		m_dataStore.addTicket(ticket);
	}
}