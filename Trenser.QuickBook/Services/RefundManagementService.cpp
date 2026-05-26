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
	const std::map<std::string, Refund*>& refunds = m_dataStore.getRefunds();
	std::ofstream refundFile(PATH, std::ios::trunc);
	if (!refundFile.is_open())
	{
		throw std::runtime_error("Cannot open file: " + PATH);
	}
	refundFile << "REFUND ID,BOOKED TICKET ID,REFUND AMOUNT,TIME,STATUS\n";
	for (std::map<std::string, Refund*>::const_iterator iterator = refunds.begin(); iterator != refunds.end(); ++iterator)
	{
		refundFile << (iterator->second)->getRefundId() << ","
			<< (iterator->second)->getBookedTicket()->getTicketId() << ","
			<< (iterator->second)->getRefundAmount() << ","
			<< (iterator->second)->getTime() << ","
			<< Enums::getRefundStatusString((iterator->second)->getStatus()) << "\n";
	}
	refundFile.close();
}