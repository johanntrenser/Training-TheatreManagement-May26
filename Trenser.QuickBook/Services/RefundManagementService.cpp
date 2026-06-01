#include "RefundManagementService.h"

/*
 * Function: RefundManagementService::RefundManagementService
 * Description: Default constructor for the RefundManagementService class.
 *              Initializes the service by retrieving the singleton instance of the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
RefundManagementService::RefundManagementService()
	: m_dataStore(DataStore::getInstance())
{
}

/*
 * Function: RefundManagementService::saveRefund
 * Description: Saves all refund data from the DataStore into a CSV file.
 *              Includes refund details such as Refund ID, Booked Ticket ID, refund amount,
 *              timestamp, and refund status.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void RefundManagementService::saveRefundData()
{
	std::vector<std::string> lines;
	lines.push_back(config::Header::REFUND_HEADER);
	const std::map<std::string, Refund*>& refunds = m_dataStore.getRefunds();
	for (std::map<std::string, Refund*>::const_iterator iterator = refunds.begin(); iterator != refunds.end(); ++iterator)
	{
		lines.push_back((iterator->second)->serialize());
	}
	FileManagement::writeLines(std::string(config::File::REFUND_FILEPATH), lines);
}

/*
 * Function: RefundManagementService::loadRefundData
 * Description: Loads all refund data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Refund object via Refund::deserialize,
 *              and restores the association with its booked Ticket if the Ticket ID
 *              is present and found in the DataStore.
 *              Finally, adds the reconstructed Refund to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void RefundManagementService::loadRefundData()
{
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	std::string refundId, bookedTicketId, refundAmount, time, status;
	for (int index = 0; index < lines.size(); ++index)
	{
		Refund* refund = Refund::deserialize(lines[index]);
		std::stringstream lineStream(lines[index]);
		getline(lineStream, refundId, ',');
		getline(lineStream, bookedTicketId, ',');
		getline(lineStream, refundAmount, ',');
		getline(lineStream, time, ',');
		getline(lineStream, status, ',');
		if (!bookedTicketId.empty())
		{
			Ticket* ticket = m_dataStore.getTicketById(bookedTicketId);
			refund->setBookedTicket(ticket);
		}
		m_dataStore.addRefund(refund);
	}
}