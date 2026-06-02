#include "DataLinker.h"

DataLinker::DataLinker() : m_dataStore(DataStore::getInstance())
{
}

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

void DataLinker::linkShowsAndSeatAvailability()
{
    const std::map<std::string, ShowSeatAvailability*>& availabilities = m_dataStore.getShowSeatAvailabilityList();
    for (std::map<std::string, ShowSeatAvailability*>::const_iterator iterator = availabilities.begin(); iterator != availabilities.end(); ++iterator)
    {
        ShowSeatAvailability* availability = iterator->second;
        if (availability != nullptr && availability->getShow() != nullptr)
        {
            availability->getShow()->setSeatAvailability(availability);
        }
    }
}

void DataLinker::linkTheatresAndMovies()
{
    const std::map<std::string, Theatre*>& theatres = m_dataStore.getTheatres();
    const std::map<std::string, Show*>& shows = m_dataStore.getShows();
    for (std::map<std::string, Theatre*>::const_iterator theatreIterator = theatres.begin(); theatreIterator != theatres.end(); ++theatreIterator)
    {
        Theatre* theatre = theatreIterator->second;
        if (theatre == nullptr)
        {
            continue;
        }
        for (std::map<std::string, Show*>::const_iterator showIterator = shows.begin(); showIterator != shows.end(); ++showIterator)
        {
            Show* show = showIterator->second;
            if (show == nullptr || show->getScreen() == nullptr || show->getMovie() == nullptr)
            {
                continue;
            }
            const Theatre* screenTheatre = show->getScreen()->getTheatre();
            if (screenTheatre != nullptr && screenTheatre->getTheatreId() == theatre->getTheatreId())
            {
                theatre->addMovieToTheatre(const_cast<Movie*>(show->getMovie()));
            }
        }
    }
}


