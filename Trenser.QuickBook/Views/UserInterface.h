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
	void selectPaymentMethod(const std::string& bookingId, double amount);
	Enums::ProcessStatus handleCardPayment(Enums::PaymentMethod type);
	Enums::ProcessStatus handleUPIPayment(Enums::PaymentMethod type);
	int displayPaymentOptions();
	void viewPaymentStatus();
	void displayPaymentStatus(const std::string& paymentId);
};
