/*
 * File: DataStore.cpp
 * Description: Defines the DataStore singleton class responsible for managing
 *              in-memory storage of system entities such as users, bookings,
 *              logs, movies, notifications, payments, refunds, screens, seats,
 *              shows, show seat availability, theatres, and tickets.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "DataStore.h"
#include <sstream>
using namespace std;

/*
 * Function: DataStore::getUsers
 * Description: Retrieves the collection of users stored in the DataStore.
 * Returns:
 *    A constant reference to the map of user IDs to User pointers.
 */
const std::map<string, User*>& DataStore::getUsers() const
{
        return m_users;
}

/*
* Function Name : getAuthenticatedUser
* Description   : Returns the currently authenticated user.
* Parameters    : None
* Return Type   : const User*
*/
const User* DataStore::getAuthenticatedUser() const
{
    return m_currentUser;
}

/*
 * Function: DataStore::addUser
 * Description: Adds a new user to the DataStore by inserting the user object
 *              into the internal map keyed by the user's unique ID.
 * Parameters:
 *    user (User*) - Pointer to the User object to be added
 * Returns:
 *    None
 */
void DataStore::addUser(User* user)
{
    m_users[user->getUserId()] = user;
}

/*
  * Function: DataStore::getInstance
  * Description: Provides access to the singleton instance of the DataStore class.
  *              Ensures only one instance exists throughout the application.
  * Returns:
  *    Reference to the single DataStore instance
  */
DataStore& DataStore::getInstance()
{
    static DataStore instance;
    return instance;
}

/*
 * Function: DataStore::setAuthenticatedUser
 * Description: Sets the currently authenticated user in the DataStore.
 * Parameters:
 *    user - Pointer to the User object representing the authenticated user
 * Returns:
 *    None
 */
void DataStore::setAuthenticatedUser(User* user)
{
    m_currentUser = user;
}

/*
* Function Name : getTickets
* Description   : Returns all tickets stored in the datastore.
* Parameters    : None
* Return Type   : std::map<std::string, Ticket*>&
*/
std::map<std::string, Ticket*>& DataStore::getTickets()
{
    return m_ticket;
}

/*
* Function Name : addTicket
* Description   : Adds a ticket to the datastore.
* Parameters    :
*                  ticket - Ticket to be added
* Return Type   : void
*/
void DataStore::addTicket(Ticket* ticket)
{
    m_ticket[ticket->getTicketId()] = ticket;
}
