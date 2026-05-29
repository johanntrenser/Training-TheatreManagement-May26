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