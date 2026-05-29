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

/*
 * Function: NotificationManagementService::loadtNotificationData
 * Description: Loads all notification data from a CSV file into memory.
 *              Reads each line from the file using FileManagement::readlines(PATH),
 *              deserializes it into a Notification object via Notification::deserialize,
 *              and restores the association with its receiver User if the receiver ID
 *              is present and found in the DataStore. Also sets the Notification status
 *              using Enums::getNotificationStatus before adding the reconstructed
 *              Notification to the DataStore.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened or read)
 */
void NotificationManagementService::loadtNotificationData()
{
	std::string notificationId, receiverId, messgae, status, time;
	std::vector<std::string> lines = FileManagement::readlines(PATH);
	for (int index = 0; index < lines.size(); ++index)
	{
		Notification* notification = Notification::deserialize(lines[index]);
		std::stringstream lineStream(lines[index]);
		getline(lineStream, notificationId, ',');
		getline(lineStream, receiverId, ',');
		getline(lineStream, messgae, ',');
		getline(lineStream, status, ',');
		getline(lineStream, time, ',');
		if (!receiverId.empty())
		{
			User* receiver = m_dataStore.getUserById(receiverId);
			notification->setReceiver(receiver);
		}
		notification->setStatus(Enums::getNotificationStatus(status));
		m_dataStore.addNotification(notification);
	}
}