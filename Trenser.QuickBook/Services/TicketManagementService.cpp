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

