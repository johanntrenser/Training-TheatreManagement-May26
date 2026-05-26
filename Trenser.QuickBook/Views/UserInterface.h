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
	void displayMovieDetails(const std::vector<const Movie*>& movies);
	void searchTheatre();
	void displayTheatresForAdmin(const std::vector<const Theatre*>& theatres);
	void displayTheatresForUsers(const std::vector<const Theatre*>& theatres);
	void listAllTheatres();
	void displayTheatres(const std::vector<const Theatre*>& theatres, Enums::TheatreStatus status);
	void addTheatre();
	void getUniqueTheatrePhoneNumber(std::string& phoneNumber);
	void getUniqueTheatreEmail(std::string& email);
	Enums::ProcessStatus handleInputTheatreDetails(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
	void displayMoviesInTheatre();
	void displayMoviesInTheatre(std::string& theatreId);
	void updateTheatre();
	Enums::ProcessStatus isValidTheatreID(const std::string& theatreId, const std::vector<const Theatre*>& theatres);
	void displayEditTheatreMenu();
	void changeTheatreName(const std::string& theatreId, const std::string& name, const Theatre*& theatre);
	void changeTheatreCity(const std::string& theatreId, const std::string& city, const Theatre*& theatre);
	void changeTheatreAddress(const std::string& theatreId, const std::string& address, const Theatre*& theatre);
	void changeTheatrePhoneNumber(const std::string& theatreId, const std::string& phoneNumber, const Theatre*& theatre);
	void changeTheatreEmail(const std::string& theatreId, const std::string& email, const Theatre*& theatre);
	const Theatre* getCurrentTheatreById(const std::string& theatreId, const std::vector<const Theatre*>& theatres);
	void validateTheatreRequest();
	void displayTheatreValidationMenu();
	void setTheatreStatusById(const std::string& theatreId, Enums::TheatreStatus theatreStatus);
	void deactivateTheatreByOwner();
	void deactivateTheatreByAdmin();
	void reactivateTheatreByOwner();
	void reactivateTheatreByAdmin();
	void addMovieToTheatre();
	void displayOwnerTheatres(const std::vector<const Theatre*>& theatres);
	const std::vector<std::string> getTheatreIds(const std::vector<const Theatre*>& theatres);
	Enums::ProcessStatus validateTheatreId(const std::string& theatreId, const std::vector<std::string>& theatreIds);
	const std::vector<std::string> getMovieIds(const std::vector<const Movie*>& movies);
	Enums::ProcessStatus validateMovieId(const std::string& movieId, const std::vector<std::string>& movieIds);
	bool isValidTime(int hour, int minute);
	bool isValidDate(int year, int month, int day);
	void getValidTime(int& hour, int& minute);
	void getValidDate(int& year, int& month, int& day);
	bool isFutureDateTime(int year, int month, int day, int hour, int minute);
	void addShow();
	bool getScreenId(const std::vector<const Screen*>& screens, std::string& screenId);
	void displayActiveShows(); 
	void displayAllShows();
	void displayShowDetails(const std::vector<const Show*> shows);
	std::string displayTimeAndDate(time_t time);
	void cancelShow();
	void viewShowStatus();
	void updateShow();
	Enums::ProcessStatus getNewDateAndTime(time_t& time);
	void listShowsForAMovie();
	void viewActiveTicketDetails();
	void viewAllTickets();
	void viewTicketHistory();
	void viewTicketDetails(const std::vector<const Ticket*>& tickets);
	void viewTicketStatus();
	void selectPaymentMethod(const std::string& bookingId, double amount);
	Enums::ProcessStatus handleCardPayment(Enums::PaymentMethod type);
	Enums::ProcessStatus handleUPIPayment(Enums::PaymentMethod type);
	int displayPaymentOptions();
	void viewPaymentStatus();
	void displayPaymentStatus(const std::string& paymentId);
};
