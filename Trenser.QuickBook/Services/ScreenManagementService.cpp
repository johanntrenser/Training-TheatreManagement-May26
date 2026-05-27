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
    /*Screen* screenss = new Screen("SC001", nullptr, "Screen123", 50, 60, {{},{}});
    m_dataStore.addScreen(screenss);
    const std::map<std::string, Screen*> screens = m_dataStore.getScreens();
    std::ofstream screenFile(PATH, std::ios::trunc);
    if (!screenFile.is_open())
    {
        throw std::runtime_error("Cannot open file: " + PATH);
    }
    screenFile << config::Header::SCREEN_HEADER<<"\n";
    for (std::map<std::string, Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        screenFile << (iterator->second)->serialize()<< "\n";
    }
    screenFile.close();*/
}