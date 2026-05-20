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
	UserInterface() : m_controller(new Controller()) {};
	void run();
	bool handleOperation(int choice);
	void login();
	void registerUser();
	void adminMenu();
	void theatreOwnerMenu();
	void customerMenu();
};
