/*
 * File: UserInterface.h
 * Description: Implements the UserInterface class responsible for handling
 *              user interactions with the Quick Book - Theatre Management System.
 * Author: Trenser
 * Created: 20 May 2026
 */
#pragma once
#include "Controller.h"
class UserInterface
{
private:
	Controller* m_controller;
public:
	UserInterface();
	void run();
	bool handleOperation(int choice);
	void login();
	void registerUser();
	void adminMenu();
	void theatreOwnerMenu();
	void customerMenu();
	void userTypesMenu();
	void handleAdminMenuOperation();
	void handleCustomerMenuOperation();
	void handleTheatreOwnerMenuOperation();
	void getUniqueEmail(std::string& email);
	void getUniquePhoneNumber(std::string& phoneNumber);
	void handleUserDetailsInput(std::string& userName, std::string& email, std::string& password, std::string& phoneNumber);
	void createUser();
	void userTypesAdminMenu();
	void viewAllUsers();
	void updateUserDetails();
	void viewProfile();
	void updateUserDetailsMenu();
	void deactivateUser();
	void viewInactiveUsers();
	void reactivateUser();
	void changePassword();
	void viewUserStatus();
	void viewNotifications();
	void viewLogsByType();
	void logsTypeMenu();
	void addMovie();
	Enums::ProcessStatus handleMovieDetailsInput(const std::string& title, const std::string& language, const std::string& genre, const int duration);
	void updateMovie();
	const std::vector<const Movie*> getMoviesByTitleInput();
	Enums::ProcessStatus validateMovieIdInput(const std::vector<const Movie*>& movies, std::string& movieId);
	void editMovieDetails(const std::string& movieId, const Movie* currentMovie);
	void displayMovie(const std::vector<const Movie*>& movies);
	const std::vector<std::string> getMovieIdFromList(const std::vector<const Movie*>& movies);
	Enums::ProcessStatus checkMovieIdIsValid(const std::string& movieId, const std::vector<std::string>& movieIdList);
	void displayCurrentMovieDetails(const std::string& movieId, const std::vector<const Movie*>& movies);
	const Movie* getCurrentMovie(const std::string& movieId, const std::vector<const Movie*>& movies);
	void displayEditMovieMenu();
	void changeMovieTitle(const std::string& movieId, const std::string& title, const Movie*& currentMovie);
	void changeMovieLanguage(const std::string& movieId, const std::string& Language, const Movie*& currentMovie);
	void changeMovieGenre(const std::string& movieId, const std::string& genre, const Movie*& currentMovie);
	void changeMovieDuration(const std::string& movieId, int& duration, const Movie*& currentMovie);
	void displayAllMovies();
	void deactivateMovie();
	void activateMovie();
	void searchMovie();
	bool displayAllInactiveMovies();
	void updateSeatLayout(Screen* screen, int rows, int columns, double amount);
	void viewSeatLayout(const Screen* screen);
	void deactivateSeat(Screen* screen, const std::string& seatId);
	void reactivateSeat(Screen* screen, const std::string& seatId);
	void viewShowSeatLayout(const Show* show);
	Enums::ProcessStatus addScreen(const std::string& theatreId, const std::string& name, int seatRows, int seatColumns, double seatAmount);
	Enums::ProcessStatus updateScreenName(const std::string& theatreId, const std::string& screenId, const std::string& name);
	Enums::ProcessStatus deactivateScreen(const std::string& theatreId, const std::string& screenId);
	Enums::ProcessStatus reactivateScreen(const std::string& theatreId, const std::string& screenId);
	void viewTheatreScreens(const std::string& theatreId);
	void viewTheatreDetails();
	void displayTheatreDetails(const std::vector<const Theatre*>& theatres);
};
