#include <iomanip>
#include <sstream>
#include "PaymentManagementService.h"
#include "TicketManagementService.h"
#include "BookingManagementService.h"
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

const std::string PaymentManagementService::generateRefundId()
{
    const std::map<std::string, Refund*>& refunds = m_dataStore.getRefunds();
    int idNumber = static_cast<int>(refunds.size()) + 1;
    std::ostringstream buffer;
    buffer << "RF" << std::setw(3) << std::setfill('0') << idNumber;
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
    Booking* booking = nullptr;
    //Booking* booking = m_dataStore.getBookingById(bookingId);
    if (booking == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::string paymentId = generatePaymentId();
    Payment* payment = Factory::getObject<Payment>(paymentId, booking, amount, paymentMethod, "nil"); //add timestamp here
    m_dataStore.addPayment(payment);
    User* currentUser = m_dataStore.getAuthenticatedUser();
    Enums::ProcessStatus status = ticketManagementService.generateTicket(payment, currentUser);
    if (status == Enums::ProcessStatus::FAILED)
    {
        return Enums::ProcessStatus::FAILED;
    }
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: PaymentManagementService::viewPaymentStatus
 * Description: Retrieves the status and details of a payment by its unique identifier.
 *              Validates that the payment exists and belongs to the currently authenticated user.
 *              Populates the provided reference parameters with booking ID, amount, payment method,
 *              payment status, and payment date if validation succeeds.
 * Parameters:
 *    paymentId     - Unique identifier of the payment to be viewed.
 *    bookingId     - Reference string to store the associated booking ID.
 *    amount        - Reference double to store the payment amount.
 *    paymentMethod - Reference enum to store the payment method used.
 *    paymentStatus - Reference enum to store the current status of the payment.
 *    paymentDate   - Reference string to store the payment date.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the payment exists, belongs to the current user,
 *    and details were successfully retrieved.
 *    Enums::ProcessStatus::FAILED if the payment does not exist or does not belong to the current user.
 */
Enums::ProcessStatus PaymentManagementService::viewPaymentStatus(const std::string& paymentId, std::string& bookingId,
    double& amount, Enums::PaymentMethod& paymentMethod, Enums::PaymentStatus& paymentStatus, std::string& paymentDate)
{
    Payment* payment = getPaymentById(paymentId);
    if (payment == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    const User* currentUser = m_dataStore.getAuthenticatedUser();
    if (payment->getBooking()->getCustomer()->getUserId() != currentUser->getUserId())
    {
        return Enums::ProcessStatus::FAILED;
    }
    bookingId = payment->getBooking()->getBookingId();
    amount = payment->getAmount();
    paymentMethod = payment->getPaymentMethod();
    paymentStatus = payment->getStatus();
    paymentDate = payment->getTimeStamp();
    return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: PaymentManagementService::refundPayment
 * Description: Processes a refund for a given payment. Validates that the payment exists,
 *              has a status of SUCCESS, and has not already been refunded. Updates the
 *              payment status to REFUNDED if all conditions are met.
 * Parameters:
 *    Ticket* - Pointer to the ticket object which is to be cancelled.
 *    Payment* - Pointer to the payment object which is to be refunded.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the payment was successfully refunded.
 *    Enums::ProcessStatus::FAILED if the payment does not exist, is already refunded,
 *    or is not in a SUCCESS state.
 */
Enums::ProcessStatus PaymentManagementService::refundPayment(Ticket* ticket, Payment* payment)
{
    if (ticket == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (payment == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (payment->getStatus() == Enums::PaymentStatus::REFUNDED)
    {
        return Enums::ProcessStatus::FAILED;
    }
    if (payment->getStatus() != Enums::PaymentStatus::SUCCESS)
    {
        return Enums::ProcessStatus::FAILED;
    }
    Booking* booking = payment->getBooking();
    if (booking == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    BookingManagementService bookingManagementService;
    Enums::ProcessStatus status = Enums::ProcessStatus::FAILED;
    //status =  bookingManagementService.cancelBooking(booking);
    if(status == Enums::ProcessStatus::FAILED)
    { 
        return Enums::ProcessStatus::FAILED;
    }
    Refund* refund = Factory::getObject<Refund>(generateRefundId(), ticket, payment->getAmount(), std::time(0));
    if (!refund)
    {
        return Enums::ProcessStatus::FAILED;
    }
    m_dataStore.addRefund(refund);
    payment->setStatus(Enums::PaymentStatus::REFUNDED);
    return Enums::ProcessStatus::SUCCESS;
}




