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
	const std::map<std::string, Ticket*>& tickets = m_dataStore.getTickets();
	std::ofstream ticketFile(PATH, std::ios::trunc);
	if (!ticketFile.is_open())
	{
		throw std::runtime_error("Cannot open file: " + PATH);
	}
	ticketFile << config::Header::TICKET_HEADER<<"\n";
	for (std::map<std::string, Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		ticketFile << (iterator->second)->serialize()<< "\n";
	}
	ticketFile.close();
}