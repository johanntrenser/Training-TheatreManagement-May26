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
	void viewTheatreDetails();
	void displayTheatreDetails(const std::vector<const Theatre*>& theatres);
	void displayMovieDetails(const std::vector<const Movie*>& movies);
	void searchTheatre();
	void  displayTheatresForAdmin(const std::vector<const Theatre*>& theatres);
	void  displayTheatresForUsers(const std::vector<const Theatre*>& theatres);
	void addTheatre();
	void getUniqueTheatrePhoneNumber(std::string& phoneNumber);
	void getUniqueTheatreEmail(std::string& email);
	Enums::ProcessStatus handleInputTheatreDetails(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
	void listAllTheatres();
	void displayTheatres(const std::vector<const Theatre*>& theatres, Enums::TheatreStatus status);
	void displayMoviesInTheatre();
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
};
	void addMovieToTheatre();
	void displayOwnerTheatres(const std::vector<const Theatre*>& theatres);
	const std::vector<std::string> getTheatreIds(const std::vector<const Theatre*>& theatres);
	Enums::ProcessStatus validateTheatreId(const std::string& theatreId, const std::vector<std::string>& theatreIds);
	const std::vector<std::string> getMovieIds(const std::vector<const Movie*>& movies);
	Enums::ProcessStatus validateMovieId(const std::string& movieId, const std::vector<std::string>& movieIds);
	void displayMovie(const std::vector<const Movie*>& movies);
};
