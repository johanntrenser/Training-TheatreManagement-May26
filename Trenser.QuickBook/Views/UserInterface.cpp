/*
 * File: UserInterface.cpp
 * Description: Implements the UserInterface class responsible for handling
 *              user interactions with the Quick Book - Theatre Management System.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;
#include "InputHelper.h"
#include "OutputHelper.h"
#include "Enums.h"
#include "Validator.h"
#include "UserInterface.h"

/*
 * Function: UserInterface::UserInterface
 * Description: Default constructor. Initializes the UserInterface by creating
 *              a new Controller instance and wiring it with all required
 *              management services including authentication, user, theatre,
 *              movie, show, booking, payment, ticket, notification, log,
 *              screen, seat, and refund management.
 * Parameters: None
 * Returns: None
 */
UserInterface::UserInterface()
{
	m_controller = new Controller(new AuthenticationManagementService(),
		new UserManagementService(),
		new TheatreManagementService(),
		new MovieManagementService(),
		new ShowManagementService(),
		new BookingManagementService(),
		new PaymentManagementService(),
		new TicketManagementService(),
		new NotificationManagementService(),
		new LogManagementService(),
		new ScreenManagementService(),
		new SeatManagementService(),
		new RefundManagementService());
}

/*
 * Function: run
 * Description: Starts the main user interface loop. Displays the menu and
 *              processes user input until the user chooses to exit.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::run()
{
	bool isMenuActive = true;
	while (isMenuActive)
	{
		try
		{
			int choice;
			util::clear();
			cout << "Quick Book - Theatre Management System\n1. Login\n2. Register\n3. Exit\nEnter Your Choice: ";
			util::readValue(choice);
			if (!handleOperation(choice))
			{
				isMenuActive = false;
			}
		}
		catch (const runtime_error& e)
		{
			cout << "Runtime Exception: " << e.what();
		}
		catch (const exception& e)
		{
			cout << "Exception : " << e.what() << endl;
			util::pressEnter();
		}
	}
}

/*
 * Function: handleOperation
 * Description: Handles the operation selected by the user from the main menu.
 * Parameters:
 *    choice - The integer value representing the user's menu selection
 * Returns:
 *    bool - true if the menu should remain active,
 *           false if the user chooses to exit
 */
bool UserInterface::handleOperation(int choice)
{
	switch (choice)
	{
	case 1:
		login();
		break;
	case 2:
		registerUser();
		break;
	case 3:
		exit();
		std::cout << "Exiting..." << std::endl;
		return false;
	default:
		cout << "Enter a valid choice!" << endl;
		util::pressEnter();
	}
	return true;
}

/*
 * Function: login
 * Description: Prompts the user for email and password, attempts authentication
 *              via the Controller, and directs the user to the appropriate menu
 *              based on their role (Admin, Theatre Owner, Customer).
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::login()
{
	string email, password;
	util::clear();
	cout << "Enter email: ";
	util::readValue(email);
	util::isEmailValid(email);
	cout << "Enter passsword: ";
	util::readValue(password);
	pair<Enums::LoginStatus, Enums::UserType> authenticationContext = m_controller->login(email, password);
	Enums::LoginStatus loginStatus = get<0>(authenticationContext);
	Enums::UserType userType = get<1>(authenticationContext);
	if (loginStatus == Enums::LoginStatus::USER_NOT_FOUND)
	{
		std::cout << "Error: User Not Found! Try Again\n";
		util::pressEnter();
		return;
	}
	if (loginStatus == Enums::LoginStatus::INVALID_PASSWORD)
	{
		std::cout << "Error: Invalid Password! Try Again\n";
		util::pressEnter();
		return;
	}
	util::clear();
	switch (userType)
	{
	case Enums::UserType::ADMIN:
	{
		handleAdminMenuOperation();
		break;
	}
	case Enums::UserType::THEATRE_OWNER:
	{
		handleTheatreOwnerMenuOperation();
		break;
	}
	case Enums::UserType::CUSTOMER:
	{
		handleCustomerMenuOperation();
		break;
	}
	}
	m_controller->logout();
};

/*
 * Function: adminMenu
 * Description: Displays the menu options available to an Admin user.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::adminMenu()
{
		cout << "Admin Menu" << endl;
		cout << "------------------------" << endl;
		cout << "1. Exit" << endl;
		cout << "Enter an option: ";
}

/*
 * Function: customerMenu
 * Description: Displays the menu options available to a Customer user.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::customerMenu()
{
	cout << "Customer Menu" << endl;
	cout << "------------------------" << endl;
	cout << "1. Exit" << endl;
	cout << "Enter an option: ";
}

/*
  * Function: theatreOwnerMenu
  * Description: Displays the menu options available to a Theatre Owner user.
  * Parameters:
  *    None
  * Returns:
  *    None
  */
void UserInterface::theatreOwnerMenu()
{
	cout << "Theatre Owner Menu" << endl;
	cout << "------------------------" << endl;
	cout << "1. Exit" << endl;
	cout << "Enter an option: ";
}

/*
 * Function: registerUser
 * Description: Prompts the user to select a user type, collects user details,
 *              and registers the user through the Controller.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::registerUser()
{
    string userName, email, password, phoneNumber;
    Enums::UserType userType = Enums::UserType::CUSTOMER;
    int choice;
    userTypesMenu();
    util::readValue(choice);
    switch (choice)
    {
    case 1:
        userType = Enums::UserType::CUSTOMER;
        break;
    case 2:
        userType = Enums::UserType::THEATRE_OWNER;
		break;
    default:
        cout << "Invalid Choice. Please Try again!" << endl;
        return;
    }
    handleUserDetailsInput(userName, email, password, phoneNumber);
    if (m_controller->registerUser(userName, email, password, phoneNumber, userType) == Enums::ProcessStatus::SUCCESS)
    {
        cout << "User registered successfully!" << endl;
		util::pressEnter();
		util::clear();
    }
    else
    {
        cout << "User could not be registered!" << endl;
		util::pressEnter();
		util::clear();
    }
}

/*
 * Function: userTypesMenu
 * Description: Displays the available user types for registration and prompts
 *              the user to make a selection.
 * Parameters:
 *    None
 * Returns:
 *    None
 */

void UserInterface::userTypesMenu()
{
    cout << "-------------------User Type List-------------------" << std::endl;
    cout << "1. Customer" << std::endl;
    cout << "2. Theatre Owner" << std::endl;
    cout << "Enter a choice: " << std::endl;
}

void UserInterface::handleAdminMenuOperation()
{
	bool isMenuActive = true;
	int choice;
	while (isMenuActive)
	{
		adminMenu();
		util::readValue(choice);
		switch (choice)
		{
		case 1:
			isMenuActive = false;
			break;
		default:
			cout << "Invalid choice. Please try again!" << endl;
			util::pressEnter();
			util::clear();
			break;
		}
	}
}

void UserInterface::handleCustomerMenuOperation()
{
	bool isMenuActive = true;
	int choice;
	while (isMenuActive)
	{
		customerMenu();
		util::readValue(choice);
		switch (choice)
		{
		case 1:
			isMenuActive = false;
			break;
		default:
			cout << "Invalid choice. Please try again!" << endl;
			util::pressEnter();
			util::clear();
			break;
		}
	}
}

void UserInterface::handleTheatreOwnerMenuOperation()
{
	bool isMenuActive = true;
	int choice;
	while (isMenuActive)
	{
		theatreOwnerMenu();
		util::readValue(choice);
		switch (choice)
		{
		case 1:
			isMenuActive = false;
			break;
		default:
			cout << "Invalid choice. Please try again!" << endl;
			util::pressEnter();
			util::clear();
			break;
		}
	}
}

/*
 * Function: UserInterface::getUniqueEmail
 * Description: Ensures that the provided email address is unique by validating
 *              against existing users in the system. Prompts the user to re‑enter
 *              the email until a unique and valid address is provided.
 * Parameters:
 *    email (std::string&) - Reference to the variable storing the user's email
 * Returns:
 *    None
 */
void UserInterface::getUniqueEmail(std::string& email)
{
	bool isEmailUnique = (m_controller->isEmailUnique(email) == Enums::ProcessStatus::SUCCESS) ? true : false;
	while (!isEmailUnique)
	{
		cout << "Email already exists!. Please enter again: ";
		util::readValue(email);
		util::isEmailValid(email);
		if (m_controller->isEmailUnique(email) == Enums::ProcessStatus::SUCCESS)
		{
			isEmailUnique = true;
		}
	}
}

/*
 * Function: UserInterface::getUniquePhoneNumber
 * Description: Ensures that the provided phone number is unique by validating
 *              against existing users in the system. Prompts the user to re‑enter
 *              the phone number until a unique and valid number is provided.
 * Parameters:
 *    phoneNumber (std::string&) - Reference to the variable storing the user's phone number
 * Returns:
 *    None
 */
void UserInterface::getUniquePhoneNumber(std::string& phoneNumber)
{
	bool isPhoneNumberUnique = (m_controller->isPhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS) ? true : false;
	while (!isPhoneNumberUnique)
	{
		cout << "Phone number already exists!. Please enter again: ";
		util::readValue(phoneNumber);
		util::isPhoneNumberValid(phoneNumber);
		if (m_controller->isPhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS)
		{
			isPhoneNumberUnique = true;
		}
	}
}

/*
 * Function: handleUserDetailsInput
 * Description: Collects and validates user details including username, email,
 *              password, and phone number. Ensures uniqueness of email and
 *              phone number before proceeding with registration.
 * Parameters:
 *    userName (string&) - Reference to the variable storing the user's name
 *    email (string&) - Reference to the variable storing the user's email
 *    password (string&) - Reference to the variable storing the user's password
 *    phoneNumber (string&) - Reference to the variable storing the user's phone number
 * Returns:
 *    None
 */
void UserInterface::handleUserDetailsInput(std::string& userName, std::string& email, std::string& password, std::string& phoneNumber)
{
    cout << "Enter user name: ";
    util::readValue(userName);
    cout << "Enter email: ";
    util::readValue(email);
    util::isEmailValid(email);
	getUniqueEmail(email);
    cout << "Enter password: ";
    util::readValue(password);
    util::isPasswordValid(password);
    cout << "Enter phone number: ";
    util::readValue(phoneNumber);
    util::isPhoneNumberValid(phoneNumber);
	getUniquePhoneNumber(phoneNumber);
}

void UserInterface::exit()
{
	m_controller->saveData();
}