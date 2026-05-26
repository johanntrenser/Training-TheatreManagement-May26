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
 *              Includes ticket details such as Ticket ID, associated Payment ID, and Customer ID.
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
	ticketFile << "TICKET ID,PAYMENT ID,CUSTOMER ID\n";
	for (std::map<std::string, Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		ticketFile << (iterator->second)->getTicketId() << ","
			<< (iterator->second)->getPayment()->getPaymentId() << ","
			<< (iterator->second)->getCustomer()->getUserId() << "\n";
	}
	ticketFile.close();
}