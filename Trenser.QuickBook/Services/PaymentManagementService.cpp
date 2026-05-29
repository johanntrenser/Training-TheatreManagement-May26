#include "PaymentManagementService.h"

/*
 * Function: PaymentManagementService::PaymentManagementService
 * Description: Default constructor for the PaymentManagementService class.
 *              Initializes the service by retrieving the singleton instance of the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
PaymentManagementService::PaymentManagementService()
    : m_dataStore(DataStore::getInstance())
{
}

/*
 * Function: PaymentManagementService::savePaymentData
 * Description: Saves all payment data from the DataStore into a CSV file.
 *              Includes payment details such as Payment ID, Booking ID, amount,
 *              payment method, status, and timestamp.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void PaymentManagementService::savePaymentData()
{
    std::vector<std::string> lines;
    lines.push_back(config::Header::PAYMENT_HEADER);
    const std::map<std::string, Payment*>& payment = m_dataStore.getPayments();
    for (std::map<std::string, Payment*>::const_iterator iterator = payment.begin(); iterator != payment.end(); ++iterator)
    {
        lines.push_back((iterator->second)->serialize());
    }
    FileManagement::writeLines(std::string(config::File::PAYMENT_FILEPATH), lines);
}