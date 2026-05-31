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
	std::vector<std::string> lines;
	lines.push_back(config::Header::NOTIFICATION_HEADER);
	const std::map<std::string, Notification*>& notifications = m_dataStore.getNotifications();
	for (std::map<std::string, Notification*>::const_iterator iterator = notifications.begin(); iterator != notifications.end(); ++iterator)
	{
		lines.push_back((iterator->second)->serialize());
	}
	FileManagement::writeLines(std::string(config::File::NOTIFICATION_FILEPATH), lines);
}