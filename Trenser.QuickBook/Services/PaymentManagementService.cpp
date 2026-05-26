#include "PaymentManagementService.h"
#include "TicketManagementService.h"
#include <iomanip>
#include <sstream>
#include "Factory.h"
#include "TimeStamp.h"

PaymentManagementService::PaymentManagementService()
    : m_dataStore(DataStore::getInstance())
{ }

/*
* Function Name : TicketManagementService::generateTicketId
* Description   : Generates a unique ticket ID based on the current number
*                 of tickets in the datastore. The ID is formatted as "USXXX".
* Parameters    : None
* Return Type : const std::string
*/
const std::string PaymentManagementService::generatePaymentId()
{
    const std::map<std::string, Payment*>& payments = m_dataStore.getPayments();
    int idNumber = static_cast<int>(payments.size()) + 1;
    std::ostringstream buffer;
    buffer << "PA" << std::setw(3) << std::setfill('0') << idNumber;
    return buffer.str();
}

/*
 * Function: PaymentManagementService::getPaymentById
 * Description: Retrieves a payment object from the DataStore by its unique identifier.
 *              Searches the internal payment map and returns the corresponding Payment
 *              pointer if a match is found.
 * Parameters:
 *    paymentId - A string representing the unique identifier of the payment to be retrieved.
 * Returns:
 *    Pointer to the Payment object if found.
 *    nullptr if no payment exists with the given ID.
 */
Payment* PaymentManagementService::getPaymentById(const std::string& paymentId)
{
    const std::map<std::string, Payment*>& payments = m_dataStore.getPayments();
    std::map<std::string, Payment*>::const_iterator iterator = payments.find(paymentId);
    if (iterator != payments.end())
    {
        return iterator->second;
    }
    return nullptr;
}

/*
 * Function: PaymentManagementService::initiatePayment
 * Description: Initiates a payment process for a given booking. Generates a unique payment ID,
 *              creates a Payment object, stores it in the DataStore, and triggers ticket generation
 *              for the authenticated user. Validates booking existence before proceeding.
 * Parameters:
 *    bookingId     - Unique identifier of the booking for which payment is to be initiated.
 *    paymentMethod - Enum value representing the chosen payment method (e.g., CARD, UPI).
 *    amount        - The payment amount to be processed.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the payment and ticket generation were successful.
 *    Enums::ProcessStatus::FAILED if the booking does not exist or ticket generation fails.
 */
Enums::ProcessStatus PaymentManagementService::initiatePayment(const std::string& bookingId, Enums::PaymentMethod paymentMethod, double amount)
{
    TicketManagementService ticketManagementService;
    Booking* booking = m_dataStore.getBookingById(bookingId);
    if (booking == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::string paymentId = generatePaymentId();
    Payment* payment = Factory::getObject<Payment>(paymentId, booking, amount, paymentMethod, Enums::PaymentStatus::SUCCESS, util::Timestamp());
    m_dataStore.addPayment(payment);
    User* currentUser = m_dataStore.getAuthenticatedUser();
    Enums::ProcessStatus status = ticketManagementService.generateTicket(payment, currentUser);
    if (status == Enums::ProcessStatus::FAILED)
    {
        return Enums::ProcessStatus::FAILED;
    }
    return Enums::ProcessStatus::SUCCESS;
}