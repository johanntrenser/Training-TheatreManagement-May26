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

/*
 * Function: PaymentManagementService::loadPaymentData
 * Description: Loads all payment data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Payment object via Payment::deserialize,
 *              and restores associations with its related Booking if the Booking ID
 *              is present and found in the DataStore. Also sets the Payment status
 *              using Enums::getPaymentStatus before adding the reconstructed Payment
 *              to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void PaymentManagementService::loadPaymentData()
{
    std::string paymentId, bookingId, amount, paymentMethod, paymentStatus, timeStamp;
    std::vector<std::string> lines = FileManagement::readlines(PATH);
    for (int index = 0; index < lines.size(); ++index)
    {
        Payment* payment = Payment::deserialize(lines[index]);
        std::stringstream lineStream(lines[index]);
        getline(lineStream, paymentId, ',');
        getline(lineStream, bookingId, ',');
        getline(lineStream, amount, ',');
        getline(lineStream, paymentMethod, ',');
        getline(lineStream, paymentStatus, ',');
        getline(lineStream, timeStamp, ',');
        if (!bookingId.empty())
        {
            Booking* booking = m_dataStore.getBookingById(bookingId);
            payment->setBooking(booking);
        }
        payment->setStatus(Enums::getPaymentStatus(paymentStatus));
        m_dataStore.addPayment(payment);
    }
}