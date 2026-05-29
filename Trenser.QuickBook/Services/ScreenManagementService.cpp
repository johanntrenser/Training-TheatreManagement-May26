#include "ScreenManagementService.h"

/*
* Function Name : ScreenManagementService
* Description   : Initializes the screen management service and connects it with the datastore.
* Parameters    : None
* Return Type   : Constructor
*/
ScreenManagementService::ScreenManagementService()
	: m_dataStore(DataStore::getInstance())
{
}

/*
 * Function: ScreenManagementService::saveScreenData
 * Description: Saves all screen data from the DataStore into a CSV file.
 *              Includes screen details such as Screen ID, Theatre ID, name, total rows, total columns,
 *              seat layout (seat IDs written row by row, separated by '|'), and screen status.
 *              Overwrites existing file content.
 * Parameters:
 *    None
 * Returns:
 *    None (throws runtime_error if the file cannot be opened)
 */
void ScreenManagementService::saveScreenData()
{
    std::vector<std::string> lines;
    lines.push_back(config::Header::SCREEN_HEADER);
    const std::map<std::string, Screen*>& screens = m_dataStore.getScreens();
    for (std::map<std::string, Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        if (iterator->second)
        {
            lines.push_back((iterator->second)->serialize());
        }
    }
    FileManagement::writeLines(std::string(config::File::SCREEN_FILEPATH), lines);
}