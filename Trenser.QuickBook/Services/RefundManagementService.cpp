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
	: m_dataStore(DataStore::getInstance()),
    m_mutex(config::MutexMappings::REFUND_MUTEX_NAME)
{
}

/*
* Function Name : RefundManagementService::getRefunds
* Description   : Retrieves refunds relevant to the currently authenticated user based on role.
*                 - For THEATRE_OWNER users: returns refunds linked to bookings in their theatres.
*                 - For CUSTOMER users: returns refunds linked to their own bookings.
* Parameters    : None
* Return Type   : const std::vector<Refund*>
*                 - A vector containing refunds relevant to the authenticated user.
*/
const std::vector<Refund*> RefundManagementService::getRefunds()
{
    ScopedLock lock(m_mutex);
    std::vector<Refund*> refunds;
    const Enums::UserType type = m_dataStore.getAuthenticatedUser()->getUserType();
    const std::string& userId = m_dataStore.getAuthenticatedUser()->getUserId();
    if (type == Enums::UserType::THEATRE_OWNER)
    {
        getTheatreRefunds(userId, refunds);
    }
    else if (type == Enums::UserType::CUSTOMER)
    {
        getCustomerRefunds(userId, refunds);
    }
    return refunds;
}

/*
* Function Name : getTheatreRefunds
* Description   : Retrieves all refunds associated with a theatre owner based on their user ID.
* Parameters    :
*     - const std::string& userId : The unique identifier of the theatre owner.
*     - std::vector<Refund*>& refunds : Reference to a vector where matching refunds will be stored.
* Return Type   : void
*/
void RefundManagementService::getTheatreRefunds(const std::string& userId, std::vector<Refund*>& refunds)
{
    ScopedLock lock(m_mutex);
    const std::map<std::string, Refund*>& allRefunds = m_dataStore.getRefunds();
    for (std::map<std::string, Refund*>::const_iterator iterator = allRefunds.begin(); iterator != allRefunds.end(); ++iterator)
    {
        Refund* refund = iterator->second;
        if (!refund ||
            !refund->getBookedTicket() ||
            !refund->getBookedTicket()->getPayment() ||
            !refund->getBookedTicket()->getPayment()->getBooking() ||
            !refund->getBookedTicket()->getPayment()->getBooking()->getShow() ||
            !refund->getBookedTicket()->getPayment()->getBooking()->getShow()->getScreen() ||
            !refund->getBookedTicket()->getPayment()->getBooking()->getShow()->getScreen()->getTheatre() ||
            !refund->getBookedTicket()->getPayment()->getBooking()->getShow()->getScreen()->getTheatre()->getTheatreOwner())
        {
            continue;
        }
        const std::string& theatreOwnerId = refund->getBookedTicket()->getPayment()->getBooking()->getShow()->getScreen()->getTheatre()->getTheatreOwner()->getUserId();
        if (theatreOwnerId == userId)
        {
            refunds.push_back(refund);
        }
    }
}

/*
* Function Name : RefundManagementService::getCustomerRefunds
* Description   : Retrieves all refunds associated with a customer based on their user ID.
* Parameters    :
*     - const std::string& userId : The unique identifier of the customer.
*     - std::vector<Refund*>& refunds : Reference to a vector where matching refunds will be stored.
* Return Type   : void
*/
void RefundManagementService::getCustomerRefunds(const std::string& userId, std::vector<Refund*>& refunds)
{
    ScopedLock lock(m_mutex);
    const std::map<std::string, Refund*>& allRefunds = m_dataStore.getRefunds();
    for (std::map<std::string, Refund*>::const_iterator iterator = allRefunds.begin(); iterator != allRefunds.end(); ++iterator)
    {
        Refund* refund = iterator->second;
        if (!refund ||
            !refund->getBookedTicket() ||
            !refund->getBookedTicket()->getCustomer())
        {
            continue;
        }
        const std::string& customerId = refund->getBookedTicket()->getCustomer()->getUserId();
        if (customerId == userId)
        {
            refunds.push_back(refund);
        }
    }
}