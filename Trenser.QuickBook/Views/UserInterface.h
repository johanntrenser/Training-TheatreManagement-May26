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
	void searchTheatre();
	void  displayTheatresForAdmin(const std::vector<const Theatre*>& theatres);
	void  displayTheatresForUsers(const std::vector<const Theatre*>& theatres);
	void addTheatre();
	void getUniqueTheatrePhoneNumber(std::string& phoneNumber);
	void getUniqueTheatreEmail(std::string& email);
	Enums::ProcessStatus handleInputTheatreDetails(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email);
	void listAllTheatres();
	void displayTheatres(const std::vector<const Theatre*>& theatres, Enums::TheatreStatus status);
};
