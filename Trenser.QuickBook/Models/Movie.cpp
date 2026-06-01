/*
 * File: Movie.cpp
 * Description: Implements the Movie class, providing constructors, getters,
 *              and setters for movie attributes such as movie ID, title,
 *              language, genre, duration, and status.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include "Movie.h"

 /*
  * Function: Movie::Movie
  * Description: Default constructor. Initializes movie attributes to empty
  *              strings or zero values, with status set to ACTIVE.
  * Returns: None
  */
Movie::Movie()
    : m_movieId(""),
    m_title(""),
    m_language(""),
    m_genre(""),
    m_duration(0),
    m_status(Enums::MovieStatus::ACTIVE)
{}

/*
 * Function: Movie::Movie
 * Description: Parameterized constructor. Initializes movie attributes with
 *              provided values. Status defaults to ACTIVE.
 * Parameters:
 *    const std::string& movieId - Unique movie identifier
 *    const std::string& title - Title of the movie
 *    const std::string& language - Language of the movie
 *    const std::string& genre - Genre of the movie
 *    int duration - Duration of the movie in minutes
 * Returns: None
 */
Movie::Movie(const std::string& movieId,
    const std::string& title,
    const std::string& language,
    const std::string& genre,
    int duration)
    : m_movieId(movieId),
    m_title(title),
    m_language(language),
    m_genre(genre),
    m_duration(duration),
    m_status(Enums::MovieStatus::ACTIVE)
{}

/*
 * Function: Movie::getMovieId
 * Description: Retrieves the movie ID.
 * Returns:
 *    const std::string& - Movie ID
 */
const std::string& Movie::getMovieId() const
{
    return m_movieId;
}

/*
 * Function: Movie::getTitle
 * Description: Retrieves the movie title.
 * Returns:
 *    const std::string& - Movie title
 */
const std::string& Movie::getTitle() const
{
    return m_title;
}

/*
 * Function: Movie::getLanguage
 * Description: Retrieves the movie language.
 * Returns:
 *    const std::string& - Movie language
 */
const std::string& Movie::getLanguage() const
{
    return m_language;
}

/*
 * Function: Movie::getGenre
 * Description: Retrieves the movie genre.
 * Returns:
 *    const std::string& - Movie genre
 */
const std::string& Movie::getGenre() const
{
    return m_genre;
}

/*
 * Function: Movie::getDuration
 * Description: Retrieves the movie duration.
 * Returns:
 *    int - Movie duration in minutes
 */
int Movie::getDuration() const
{
    return m_duration;
}

/*
 * Function: Movie::getStatus
 * Description: Retrieves the movie status.
 * Returns:
 *    Enums::MovieStatus - Current movie status
 */
Enums::MovieStatus Movie::getStatus() const
{
    return m_status;
}

/*
 * Function: Movie::setMovieId
 * Description: Updates the movie ID.
 * Parameters:
 *    const std::string& id - New movie ID
 * Returns: None
 */
void Movie::setMovieId(const std::string& id)
{
    m_movieId = id;
}

/*
 * Function: Movie::setTitle
 * Description: Updates the movie title.
 * Parameters:
 *    const std::string& title - New movie title
 * Returns: None
 */
void Movie::setTitle(const std::string& title)
{
    m_title = title;
}

/*
 * Function: Movie::setLanguage
 * Description: Updates the movie language.
 * Parameters:
 *    const std::string& language - New movie language
 * Returns: None
 */
void Movie::setLanguage(const std::string& language)
{
    m_language = language;
}

/*
 * Function: Movie::setGenre
 * Description: Updates the movie genre.
 * Parameters:
 *    const std::string& genre - New movie genre
 * Returns: None
 */
void Movie::setGenre(const std::string& genre)
{
    m_genre = genre;
}

/*
 * Function: Movie::setDuration
 * Description: Updates the movie duration.
 * Parameters:
 *    int duration - New movie duration in minutes
 * Returns: None
 */
void Movie::setDuration(int duration)
{
    m_duration = duration;
}

/*
 * Function: Movie::setStatus
 * Description: Updates the movie status.
 * Parameters:
 *    Enums::MovieStatus status - New movie status
 * Returns: None
 */
void Movie::setStatus(Enums::MovieStatus status)
{
    m_status = status;
}

/*
 * Function: serialize
 * Description: Converts Movie object into CSV format string
 * Returns:
 *    CSV string representing the user
 */
std::string Movie::serialize()
{
    return m_movieId + config::delimeter::comma +
        m_title + config::delimeter::comma +
        m_language + config::delimeter::comma +
        m_genre + config::delimeter::comma +
        std::to_string(m_duration) + config::delimeter::comma +
        Enums::getMovieStatusString(m_status);
}

/*
 * Function: Movie::deserialize
 * Description: Converts a single CSV-formatted line into a Movie object.
 *              Extracts fields such as movieId, title, language, genre,
 *              duration, and status. The duration string is converted
 *              into an integer using stoi. The Movie status and any
 *              associations (e.g., with Theatre or Shows) are initialized
 *              separately by higher-level services after deserialization.
 * Parameters:
 *    lines - reference to a CSV-formatted string containing movie data
 * Returns:
 *    Pointer to a newly constructed Movie object
 */
Movie* Movie::deserialize(std::string& lines)
{
    std::string movieId, title, language, genre, duration, status;
    std::stringstream lineStream(lines);
    getline(lineStream, movieId, ',');
    getline(lineStream, title, ',');
    getline(lineStream, language, ',');
    getline(lineStream, genre, ',');
    getline(lineStream, duration, ',');
    getline(lineStream, status, ',');
    return new Movie(movieId, title, language, genre, stoi(duration));
}