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
* Function Name : generatePaymentId
* Description   : Generates a unique payment ID based on the current number
*                 of payments in the datastore. The ID is formatted as "PAXXX".
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
* Function Name : generateRefundId
* Description   : Generates a unique refund ID based on the current number
*                 of refunds in the datastore. The ID is formatted as "RFXXX".
* Parameters    : None
* Return Type : const std::string
*/
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
* Function Name : initiatePayment
* Description   : Creates a payment for the specified booking, generates
*                 the associated ticket, and updates the booking status
*                 upon successful completion. Rolls back the booking if
*                 payment processing or ticket generation fails.
* Parameters    :
*                  bookingId     - Unique identifier of the booking
*                  paymentMethod - Selected payment method
*                  amount        - Payment amount
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus PaymentManagementService::initiatePayment(const std::string& bookingId, Enums::PaymentMethod paymentMethod, double amount)
{
    std::string message;
    TicketManagementService ticketManagementService;
    BookingManagementService bookingManagementService;
    User* currentUser = m_dataStore.getAuthenticatedUser();
    Booking* booking = m_dataStore.getBookingByIdForUpdation(bookingId);
    if (booking == nullptr)
    {
        return Enums::ProcessStatus::FAILED;
    }
    std::string paymentId = generatePaymentId();
    Payment* payment = Factory::getObject<Payment>(paymentId, booking, amount, paymentMethod, std::time(nullptr));
    if (payment == nullptr)
    {
        bookingManagementService.cancelBookingForFailedPayment(bookingId);
        message = "Payment of customer with ID : " + currentUser->getUserId() + " has failed";
        logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
        return Enums::ProcessStatus::FAILED;
    }
    Enums::ProcessStatus status = ticketManagementService.generateTicket(payment, currentUser);
    if (status == Enums::ProcessStatus::FAILED)
    {
        bookingManagementService.cancelBookingForFailedPayment(booking->getBookingId());
        message = "Payment of customer with ID : " + currentUser->getUserId() + " has failed";
        logManagementService.addLog(message, Enums::LogType::ERROR_LOG);
        return Enums::ProcessStatus::FAILED;
    }
    payment->setStatus(Enums::PaymentStatus::SUCCESS);
    m_dataStore.addPayment(payment);
    booking->setStatus(Enums::BookingStatus::CONFIRMED);
    message = "Payment with ID : " + payment->getPaymentId() + " has been completetd";
    logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
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
    Refund* refund = Factory::getObject<Refund>(generateRefundId(), ticket, payment->getAmount(), std::time(0));
    if (!refund)
    {
        return Enums::ProcessStatus::FAILED;
    }
    m_dataStore.addRefund(refund);
    payment->setStatus(Enums::PaymentStatus::REFUNDED);
    ticket->setTicketStatus(Enums::TicketStatus::CANCELLED);
    std::string message = "Payment with ID : " + payment->getPaymentId() + " has been refunded.";
    logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    message = "Your refund request for booking with id " + booking->getBookingId() + " has been processed successfully";
    m_notificationManagementService.sendNotification(ticket->getCustomer(), message);
    return Enums::ProcessStatus::SUCCESS;
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
    for (int index = 1; index < lines.size(); ++index)
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
            Booking* booking = m_dataStore.getBookingDetailsById(bookingId);
            payment->setBooking(booking);
        }
        payment->setStatus(Enums::getPaymentStatus(paymentStatus));
        m_dataStore.addPayment(payment);
    }
}

/*
* Function Name : PaymentManagementService::getAllPayments
* Description   : Retrieves all payments from the datastore based on the authenticated user’s role.
*                 - For CUSTOMER users: returns only payments linked to their own bookings.
*                 - For THEATRE_OWNER users: returns payments linked to shows in their theatres.
*                 Performs null checks at each level (Payment, Booking, Show, Screen, Theatre, User)
*                 to ensure safe traversal of relationships.
* Parameters    : None
* Return Type   : const std::vector<Payment*>
*                 - A vector containing payments relevant to the authenticated user.
*/
const std::vector<Payment*> PaymentManagementService::getAllPayments()
{
    const std::map<std::string, Payment*> allPayments = m_dataStore.getPayments();
    const User* authenticatedUser = m_dataStore.getAuthenticatedUser();
    std::vector<Payment*> currentCustomerPayments;
    if (authenticatedUser == nullptr)
    {
        return currentCustomerPayments;
    }
    if (authenticatedUser->getUserType() == Enums::UserType::CUSTOMER)
    {
        for (std::map<std::string, Payment*>::const_iterator iterator = allPayments.begin(); iterator != allPayments.end(); ++iterator)
        {
            const Payment* payment = iterator->second;
            if (payment == nullptr)
            {
                continue;
            }
            const Booking* booking = payment->getBooking();
            if (booking == nullptr)
            {
                continue;
            }
            const User* customer = booking->getCustomer();
            if (customer == nullptr)
            {
                continue;
            }
            if (customer->getUserId() == authenticatedUser->getUserId())
            {
                currentCustomerPayments.push_back(iterator->second);
            }
        }
    }
    else if (authenticatedUser->getUserType() == Enums::UserType::THEATRE_OWNER)
    {
        for (std::map<std::string, Payment*>::const_iterator iterator = allPayments.begin(); iterator != allPayments.end(); ++iterator)
        {
            const Payment* payment = iterator->second;
            if (payment == nullptr)
            {
                continue;
            }
            const Booking* booking = payment->getBooking();
            if (booking == nullptr)
            {
                continue;
            }
            const Show* show = booking->getShow();
            if (show == nullptr)
            {
                continue;
            }
            const Screen* screen = show->getScreen();
            if (screen == nullptr)
            {
                continue;
            }
            const Theatre* theatre = screen->getTheatre();
            if (theatre == nullptr)
            {
                continue;
            }
            const User* theatreOwner = theatre->getTheatreOwner();
            if (theatreOwner == nullptr)
            {
                continue;
            }
            if (theatreOwner->getUserId() == authenticatedUser->getUserId())
            {
                currentCustomerPayments.push_back(iterator->second);
            }
        }
    }
    return currentCustomerPayments;
}
