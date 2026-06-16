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
 * Function: Movie::serialize
 * Description: Converts a Movie object into a SharedMovie structure suitable for
 *              storage in shared memory. Copies string attributes (movieId, title,
 *              language, genre) into fixed-size character arrays using strncpy_s
 *              to ensure safe buffer handling. Stores duration and status as
 *              primitive values. This serialized representation allows the Movie
 *              to be persisted and retrieved across processes.
 * Parameters:
 *    None
 * Returns:
 *    A SharedMovie structure containing the serialized data of the Movie object.
 */
SharedMovie Movie::serialize()
{
    SharedMovie sharedMovie{};
    strncpy_s(sharedMovie.movieId, m_movieId.c_str(), sizeof(sharedMovie.movieId));
    strncpy_s(sharedMovie.title, m_title.c_str(), sizeof(sharedMovie.title));
    strncpy_s(sharedMovie.language, m_language.c_str(), sizeof(sharedMovie.language));
    strncpy_s(sharedMovie.genre, m_genre.c_str(), sizeof(sharedMovie.genre));
    sharedMovie.duration = m_duration;
    sharedMovie.status = static_cast<int>(m_status);
    return sharedMovie;
}

/*
 * Function: Movie::deserialize
 * Description: Converts a SharedMovie record from shared memory into a fully constructed
 *              Movie object. Validates that the input pointer is not null, extracts
 *              attributes such as movieId, title, language, genre, duration, and status,
 *              and uses the Factory to instantiate a Movie object. The Movie status is
 *              then set based on the deserialized value. This ensures symmetry with
 *              Movie::serialize for round-trip persistence.
 * Parameters:
 *    sharedMovie - A pointer to a SharedMovie structure containing serialized movie data.
 * Returns:
 *    A pointer to a newly constructed Movie object, or nullptr if the input is null.
 */
Movie* Movie::deserialize(const SharedMovie* sharedMovie)
{
    if (sharedMovie == nullptr)
    {
        return nullptr;
    }
    Enums::MovieStatus status = static_cast<Enums::MovieStatus>(sharedMovie->status);
    Movie* movie = Factory::getObject<Movie>(
        sharedMovie->movieId,
        sharedMovie->title,
        sharedMovie->language,
        sharedMovie->genre,
        sharedMovie->duration
    );
    if(movie!=nullptr)
    {
        movie->setStatus(status);
    }
    return movie;
}