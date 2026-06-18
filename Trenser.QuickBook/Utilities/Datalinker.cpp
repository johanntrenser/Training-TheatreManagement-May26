#include "DataLinker.h"

DataLinker::DataLinker() : m_dataStore(DataStore::getInstance())
{
}

/*
Function Name : linkTheatresAndScreens
Description   : Iterates through all screens in the datastore and links each screen
                to its corresponding theatre by setting the screen reference in the theatre.
Parameters    : None
Return Type   : void
*/
void DataLinker::linkTheatresAndScreens()
{
    const std::map<std::string, Screen*>& screens = m_dataStore.getScreens();
    for (std::map<std::string, Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
    {
        Screen* screen = iterator->second;
        if (screen != nullptr && screen->getTheatre() != nullptr)
        {
            Theatre* theatre = const_cast<Theatre*>(screen->getTheatre());
            theatre->setScreen(screen);
        }
    }
}

/*
Function Name : linkShowsAndSeatAvailability
Description   : Iterates through all show seat availability records in the datastore
                and links each availability to its corresponding show by setting the
                seat availability reference in the show.
Parameters    : None
Return Type   : void
*/
void DataLinker::linkShowsAndSeatAvailability()
{
    const std::map<std::string, ShowSeatAvailability*>& availabilities = m_dataStore.getShowSeatAvailabilitys();
    for (std::map<std::string, ShowSeatAvailability*>::const_iterator iterator = availabilities.begin(); iterator != availabilities.end(); ++iterator)
    {
        ShowSeatAvailability* availability = iterator->second;
        if (availability != nullptr && availability->getShow() != nullptr)
        {
            availability->getShow()->setSeatAvailability(availability);
        }
    }
}


