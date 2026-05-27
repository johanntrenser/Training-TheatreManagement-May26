#include "NotificationManagementService.h"

/*
* Function Name : NotificationManagementService
* Description   : Initializes the notification management service and connects it with the datastore.
* Parameters    : None
* Return Type   : Constructor
*/
NotificationManagementService::NotificationManagementService()
	: m_dataStore(DataStore::getInstance())
{
}

/*
 * Function: NotificationManagementService::saveNotificationData
 * Description: Saves all notification data from the DataStore into a CSV file.
 *              Includes notification details such as Notification ID, Receiver ID,
 *              message content, status, and timestamp.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void NotificationManagementService::saveNotificationData()
{
	const std::map<std::string, Notification*>& notifications = m_dataStore.getNotifications();
	std::ofstream notificationFile(PATH, std::ios::trunc);
	if (!notificationFile.is_open())
	{
		throw std::runtime_error("Cannot open file: " + PATH);
	}
	notificationFile << config::Header::NOTIFICATION_HEADER;
	for (std::map<std::string, Notification*>::const_iterator iterator = notifications.begin(); iterator != notifications.end(); ++iterator)
	{
		notificationFile << (iterator->second)->serialize() << "\n";
	}
	notificationFile.close();
}