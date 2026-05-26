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
    const std::map<std::string, Payment*>& payment = m_dataStore.getPayments();
    std::ofstream paymentFile(PATH, std::ios::trunc);
    if (!paymentFile.is_open())
    {
        throw std::runtime_error("Cannot open file: " + PATH);
    }
    paymentFile << "PAYMENT ID,BOOKING ID,AMOUNT,PAYMENT METHOD,STATUS,TIME STAMP\n";
    for (std::map<std::string, Payment*>::const_iterator iterator = payment.begin(); iterator != payment.end(); ++iterator)
    {
        paymentFile << (iterator->second)->getPaymentId() << ","
            << (iterator->second)->getBooking()->getBookingId() << ","
            << (iterator->second)->getAmount() << ","
            << Enums::getPaymentMethodString((iterator->second)->getPaymentMethod()) << ","
            << Enums::getPaymentStatusString((iterator->second)->getStatus()) << ","
            << (iterator->second)->getTimeStamp()<<"\n";
    }
    paymentFile.close();
}