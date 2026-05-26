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
	bool isValidTime(int hour, int minute);
	bool isValidDate(int year, int month, int day);
	void getValidTime(int& hour, int& minute);
	void getValidDate(int& year, int& month, int& day);
	bool isFutureDateTime(int year, int month, int day, int hour, int minute);
	void addShow();
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
	void displayMoviesInTheatre(std::string& theatreId);
	bool getScreenId(const std::vector<const Screen*>& screens, std::string& screenId);
	void updateTheatre();
	Enums::ProcessStatus isValidTheatreID(const std::string& theatreId, const std::vector<const Theatre*>& theatres);
	void displayEditTheatreMenu();
	void changeTheatreName(const std::string& theatreId, const std::string& name, const Theatre*& theatre);
	void changeTheatreCity(const std::string& theatreId, const std::string& city, const Theatre*& theatre);
	void changeTheatreAddress(const std::string& theatreId, const std::string& address, const Theatre*& theatre);
	void changeTheatrePhoneNumber(const std::string& theatreId, const std::string& phoneNumber, const Theatre*& theatre);
	void changeTheatreEmail(const std::string& theatreId, const std::string& email, const Theatre*& theatre);
	const Theatre* getCurrentTheatreById(const std::string& theatreId, const std::vector<const Theatre*>& theatres);
	void displayActiveShows();
	void displayShowDetails(const std::vector<const Show*> shows);
	std::string displayTimeAndDate(time_t time);
	void cancelShow();
};