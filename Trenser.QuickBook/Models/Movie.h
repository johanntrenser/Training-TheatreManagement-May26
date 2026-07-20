/*
 * File: Movie.h
 * Description: Implements the Movie class, providing constructors, getters,
 *              and setters for movie attributes such as movie ID, title,
 *              language, genre, duration, and status.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include <string>
#include <sstream>
#include "ApplicationConfig.h"
#include "Enums.h"
#include "Factory.h"
#include "SharedStructs.h"

class Movie
{
private:
    std::string m_movieId;
    std::string m_title;
    std::string m_language;
    std::string m_genre;
    int m_duration;
    Enums::MovieStatus m_status;
public:
    Movie();
    Movie(const std::string& movieId,
        const std::string& title,
        const std::string& language,
        const std::string& genre,
        int duration);
    const std::string& getMovieId() const;
    const std::string& getTitle() const;
    const std::string& getLanguage() const;
    const std::string& getGenre() const;
    int getDuration() const;
    Enums::MovieStatus getStatus() const;
    void setMovieId(const std::string& id);
    void setTitle(const std::string& title);
    void setLanguage(const std::string& language);
    void setGenre(const std::string& genre);
    void setDuration(int duration);
    void setStatus(Enums::MovieStatus status);
    SharedMovie serialize();
    static Movie* deserialize(const SharedMovie* sharedMovie);
};