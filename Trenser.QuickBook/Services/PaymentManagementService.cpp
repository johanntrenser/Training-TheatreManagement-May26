#include <iomanip>
#include <sstream>
#include "PaymentManagementService.h"
#include "TicketManagementService.h"
#include "BookingManagementService.h"
#include "Factory.h"
#include "TimeStamp.h"
#include "ApplicationConfig.h"

PaymentManagementService::PaymentManagementService()
    : m_dataStore(DataStore::getInstance()),
    m_paymentMutex(config::MutexMappings::PAYMENT_MUTEX_NAME),
    m_refundMutex(config::MutexMappings::REFUND_MUTEX_NAME)
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
    const int paymentCount = m_dataStore.getPaymentCount();
    int idNumber = paymentCount + 1;
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
    const int refundCount = m_dataStore.getRefundCount();
    int idNumber = refundCount + 1;
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
    ScopedLock lock(m_paymentMutex);
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
    ScopedLock lock(m_paymentMutex);
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
    m_dataStore.updatePaymentStatus(payment->getPaymentId(), Enums::PaymentStatus::SUCCESS);
    payment->setStatus(Enums::PaymentStatus::SUCCESS);
    m_dataStore.addPayment(payment);
    booking->setStatus(Enums::BookingStatus::CONFIRMED);
    message = "Payment with ID : " + payment->getPaymentId() + " has been completed";
    logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    std::string notificationMessage = "Ticket is successfully booked ";
    m_event.notify("", m_dataStore.getAuthenticatedUser()->getUserId(), notificationMessage);
    m_notificationManagementService.sendNotification(m_dataStore.getAuthenticatedUser(), message);
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
    ScopedLock paymentLock(m_paymentMutex);
    ScopedLock refundLock(m_refundMutex);
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
    m_dataStore.updatePaymentStatus(payment->getPaymentId(), Enums::PaymentStatus::REFUNDED);
    payment->setStatus(Enums::PaymentStatus::REFUNDED);
    ticket->setTicketStatus(Enums::TicketStatus::CANCELLED);
    std::string message = "Payment with ID : " + payment->getPaymentId() + " has been refunded.";
    logManagementService.addLog(message, Enums::LogType::SYSTEM_ACTIVITY);
    message = "Your refund request for booking with id " + booking->getBookingId() + " has been processed successfully";
    m_notificationManagementService.sendNotification(ticket->getCustomer(), message);
    m_event.notify("", m_dataStore.getAuthenticatedUser()->getUserId(), message);
    return Enums::ProcessStatus::SUCCESS;
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
    ScopedLock lock(m_paymentMutex);
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
