#include "ScreenManagementService.h"

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

/*
 * name        : loadScreenData
 * description : Loads screen data from the CSV file, deserializes each line into a Screen object,
 *               resolves references to Theatre, sets status, and adds the Screen to datastore.
 * parameter   : None
 * return type : void
 */
void ScreenManagementService::loadScreenData()
{
    std::string screenId, theatreId, name, totalRows, totalColumns, status;
    std::vector<std::string> lines = FileManagement::readlines(PATH);
    for (int index = 0; index < lines.size(); index++)
    {
        Screen* screen = Screen::deserialize(lines[index]);
        std::stringstream lineStream(lines[index]);
        getline(lineStream, screenId, ',');
        getline(lineStream, theatreId, ',');
        getline(lineStream, name, ',');
        getline(lineStream, totalRows, ',');
        getline(lineStream, totalColumns, ',');
        getline(lineStream, status, ',');
        if (!theatreId.empty())
        {
            Theatre* theatre = m_dataStore.getTheatreById(theatreId);
            screen->setTheatre(theatre);
        }
        Enums::ScreenStatus screenStatus = Enums::getScreenStatus(status);
        screen->setScreenStatus(screenStatus);
        m_dataStore.addScreen(screen);
    }
}