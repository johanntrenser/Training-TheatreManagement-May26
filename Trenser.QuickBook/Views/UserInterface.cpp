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
#include<iomanip>
#include <ctime>
#include <sstream>
using namespace std;
#include "InputHelper.h"
#include "OutputHelper.h"
#include "UserInterface.h"
#include "Validator.h"
#include "Enums.h"
#include "TimeStamp.h"

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
	cout << "Enter password: ";
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
	cout << " 1.  Create User" << endl;
	cout << " 2.  View All Users" << endl;
	cout << " 3.  Update User Details" << endl;
	cout << " 4.  Deactivate User" << endl;
	cout << " 5.  Reactivate User" << endl;
	cout << " 6.  View Profile" << endl;
	cout << " 7.  View User Status" << endl;
	cout << " 8.  Change Password" << endl;
	cout << " 9.  Add Movie" << endl;
	cout << "10.  Update Movie Details" << endl;
	cout << "11.  List All Movies" << endl;
	cout << "12.  Deactivate Movie" << endl;
	cout << "13.  Reactivate Movie" << endl;
	cout << "14.  Search Movie" << endl;
	cout << "15. Validate Theatre Request" << endl;
	cout << "16. Deactivate theatre byadmin" << endl;
	cout << "17. Logout" << endl;
	cout << "------------------------" << endl;
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
	cout << " 1.  Update User Details" << endl;
	cout << " 2.  View Profile" << endl;
	cout << " 3.  Change Password" << endl;
	cout << " 4.  Search Movie" << endl;
	cout << " 5.  Logout" << endl;
	cout << "------------------------" << endl;
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
	cout << " 1.  Add Theatre" << endl;
	cout << " 2.  View Theatre Details" << endl;
	cout << " 3.  Update Theatre Details" << endl;
	cout << " 4.  Deactivate Theatre" << endl;
	cout << " 5.  Reactivate Theatre" << endl;
	cout << " 6.  Search Theatre" << endl;
	cout << " 7.  List All Theatres" << endl;
	cout << " 8.  Add Movie to Theatre" << endl;
	cout << " 9.  View Movies in Theatre" << endl;
	cout << "10.  Update User Details" << endl;
	cout << "11.  View Profile" << endl;
	cout << "12.  Change Password" << endl;
	cout << "13.  Logout" << endl;
	cout << "------------------------" << endl;
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
		util::pressEnter();
		util::clear();
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

/*
 * Function: UserInterface::handleAdminMenuOperation
 * Description: Handles the admin menu operations in a loop until the admin exits.
 *              Displays the admin menu, reads the choice, and executes the corresponding
 *              operation such as validating theatre requests, deactivating theatres,
 *              or reactivating theatres. Provides feedback for invalid choices.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
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
			createUser();
			break;
		case 2:
			viewAllUsers();
			break;
		case 3:
			updateUserDetails();
			break;
		case 4:
			deactivateUser();
			break;
		case 5:
			reactivateUser();
			break;
		case 6:
			viewProfile();
			break;
		case 7:
			viewUserStatus();
			break;
		case 8:
			changePassword();
			break;
		case 9:
			addMovie();
			break;
		case 10:
			updateMovie();
			break;
		case 11:
			displayAllMovies();
			break;
		case 12:
			deactivateMovie();
			break;
		case 13:
			activateMovie();
			break;
		case 14:
			searchMovie();
			break;
		case 15:
			validateTheatreRequest();
			break;
		case 16:
			deactivateTheatreByAdmin();
			break;
		case 17:
			m_controller->logout();
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
 * Function: UserInterface::handleCustomerMenuOperation
 * Description: Handles the customer menu operations in a loop until the customer exits.
 *              Displays the customer menu, reads the choice, and executes the corresponding
 *              operation. Currently supports exiting the menu and provides feedback for invalid choices.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
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
			updateUserDetails();
			break;
		case 2:
			viewProfile();
			break;
		case 3:
			changePassword();
			break;
		case 4:
			searchMovie();
			break;
		case 5:
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
 * Function: UserInterface::handleTheatreOwnerMenuOperation
 * Description: Handles the theatre owner menu operations in a loop until the owner exits.
 *              Displays the theatre owner menu, reads the choice, and executes the corresponding
 *              operation such as adding theatres, viewing details, updating, deactivating/reactivating,
 *              managing movies, searching, and listing theatres. Provides feedback for invalid choices.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
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
			// Theatre Management
		case 1:
			addTheatre();
			break;
		case 2:
			viewTheatreDetails();
			break;
		case 3:
			updateTheatre();
			break;
		case 4:
			deactivateTheatreByOwner();
			break;
		case 5:
			reactivateTheatreByOwner();
			break;
		case 6:
			searchTheatre();
			break;
		case 7:
			listAllTheatres();
			break;
		case 8:
			addMovieToTheatre();
			break;
		case 9:
			displayMoviesInTheatre();
			break;
		case 10:
			updateUserDetails();
			break;
		case 11:
			viewProfile();
			break;
		case 12:
			changePassword();
			break;
		case 13:
			m_controller->logout();
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
 * Function: UserInterface::updateSeatLayout
 * Description: Prompts the user to enter the number of rows and columns for a
 *              given screen’s seating layout. Passes the update request to
 *              the Controller
 * Parameters:
 *    screen  - A pointer to the Screen object whose seat layout is to be updated
 *    rows    - The number of rows in the seating layout
 *    columns - The number of columns in the seating layout
 * Returns:
 *    None
 */
void UserInterface::updateSeatLayout(Screen* screen, int rows, int columns, double amount)
{
	cout << "Enter Number of Rows: ";
	util::readValue(rows);
	cout << "Enter Number of Columns: ";
	util::readValue(columns);
	if (m_controller->updateSeatLayout(screen, rows, columns, amount) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Seat Layout Updated Successfully" << endl;
	}
	else
	{
		cout << "Failed! Seat Layout could not be updated" << endl;
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

/*
 * Function: createUser
 * Description: Allows an Admin to register a new user directly by selecting
 *              the role (Customer, Theatre Owner, or Admin) and entering
 *              user details.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::createUser()
{
	string userName, email, password, phoneNumber;
	Enums::UserType userType = Enums::UserType::CUSTOMER;
	int choice;
	userTypesAdminMenu();
	util::readValue(choice);
	switch (choice)
	{
	case 1:
		userType = Enums::UserType::CUSTOMER;
		break;
	case 2:
		userType = Enums::UserType::THEATRE_OWNER;
		break;
	case 3:
		userType = Enums::UserType::ADMIN;
		break;
	default:
		cout << "Invalid Choice. Please Try again!" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	handleUserDetailsInput(userName, email, password, phoneNumber);
	if (m_controller->createUser(userName, email, password, phoneNumber, userType) == Enums::ProcessStatus::SUCCESS)
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
 * Function: userTypesAdminMenu
 * Description: Displays the list of available user types (Customer, Theatre Owner,
 *              Admin) for Admin registration.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::userTypesAdminMenu()
{
	cout << "-------------------User Type List-------------------" << std::endl;
	cout << "1. Customer" << std::endl;
	cout << "2. Theatre Owner" << std::endl;
	cout << "3. Admin" << std::endl;
	cout << "Enter a choice: " << std::endl;
}

/*
 * Function: UserInterface::updateUserDetails
 * Description: Provides a menu for the authenticated user to update
 *              their details (username, email, phone number). Prompts the user
 *              for each field individually, validates input, and passes the
 *              update request to the Controller.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::updateUserDetails()
{
	bool condition = true;
	while (condition)
	{
		int choice;
		string input;
		Enums::ProcessStatus result = Enums::ProcessStatus::FAILED;
		viewProfile();
		updateUserDetailsMenu();
		util::readValue(choice);
		switch (choice)
		{
		case 1:
			cout << "Enter username: ";
			util::readValue(input);
			result = m_controller->setAuthenticatedUserUserName(input);
			break;
		case 2:
			cout << "Enter email: ";
			util::readValue(input);
			result = m_controller->setAuthenticatedUserEmail(input);
			break;
		case 3:
			cout << "Enter phoneNumber: ";
			util::readValue(input);
			result = m_controller->setAuthenticatedUserPhoneNumber(input);
			break;
		case 4:
			condition = false;
			break;
		default:
			cout << "Enter a valid option!" << endl;
			util::pressEnter();
			util::clear();
			break;
		}
		if (choice >= 1 && choice <= 3)
		{
			if (result == Enums::ProcessStatus::SUCCESS)
			{
				cout << "user Details Updated Successfully" << endl;
			}
			else
			{
				cout << "failed, could not update user details!" << endl;
			}
			util::pressEnter();
			util::clear();
		}
	}
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::updateUserDetailsMenu
 * Description: Displays the update user details menu options (username, email,
 *              phone number, exit) and prompts the user to select an option.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::updateUserDetailsMenu()
{
	cout << "<--- User Details Menu---->" << endl;
	cout << "1. User Name\n2. Email\n3. Phone Number\n4. Exit\nEnter Your Choice: ";
}

/*
 * Function: UserInterface::deactivateUser
 * Description: Displays all users, prompts the Admin to enter a User ID,
 *              and attempts to deactivate the selected user. Provides feedback on
 *              whether the deactivation was successful or failed.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::deactivateUser()
{
	string userId;
	viewAllUsers();
	cout << "Enter the User ID: ";
	util::readValue(userId);
	Enums::ProcessStatus result = m_controller->deactivateUser(userId);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		cout << "USER " << userId << " deactivated successfully" << endl;
	}
	else
	{
		cout << "Invalid User Id, Try again!" << endl;
	}
	util::pressEnter();
	util::clear();
}

/*
* Function Name : viewNotifications
* Description   : Displays unread notifications for the authenticated user in batches.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewNotifications()
{
	bool condition = true;
	int choice = 1;
	int count = 0;
	while (condition)
	{
		int remainingUnreadCount = 0;
		const std::vector<std::string> notifications = m_controller->getUnreadNotifications(5, remainingUnreadCount);
		if (notifications.empty())
		{
			cout << "No unread notifications." << endl;
			return;
		}
		cout << endl;
		cout << "========= Notifications =========" << endl << endl;
		for (std::vector<std::string>::const_iterator iterator = notifications.begin(); iterator != notifications.end(); ++iterator)
		{
			cout << ++count << ". " << (*iterator) << endl;
		}
		if (remainingUnreadCount > 0)
		{
			cout << remainingUnreadCount << " unread notifications remaining." << endl;
			cout << "1. View More" << endl;
			cout << "2. Exit" << endl;
			util::readValue(choice);
			if (choice == 2)
			{
				condition = false;
			}
		}
		else
		{
			cout << "No more unread notifications." << endl;
			break;
		}
	}
}

/*
 * Function: UserInterface::viewInactiveUsers
 * Description: Retrieves all inactive users from the Controller and displays
 *              them in a tabular format on the console.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::viewInactiveUsers()
{
	const vector<const User*> users = m_controller->getInactiveUsers();
	if (users.empty())
	{
		cout << "No users found!" << endl;
	}
	std::cout << std::left
		<< std::setw(10) << "User ID"
		<< std::setw(20) << "Name"
		<< std::setw(25) << "Email"
		<< std::setw(15) << "Password"
		<< std::setw(15) << "Role"
		<< std::endl;
	std::cout << std::string(100, '-') << std::endl;
	for (const User* const user : users)
	{
		std::cout << std::left
			<< std::setw(10) << user->getUserId()
			<< std::setw(20) << user->getUserName()
			<< std::setw(25) << user->getEmail()
			<< std::setw(15) << user->getPassword()
			<< std::setw(15) << Enums::getUserTypeString(user->getUserType())
			<< std::endl;
	}
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::handleMovieDetailsInput
 * Description: Validates whether the provided movie details represent a unique movie in the system.
 * Parameters:
 *    title    - Title of the movie
 *    language - Language of the movie
 *    genre    - Genre of the movie
 *    duration - Duration of the movie in minutes
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie is unique,
 *    Enums::ProcessStatus::FAILED if a duplicate exists
 */
Enums::ProcessStatus UserInterface::handleMovieDetailsInput(const std::string& title, const std::string& language, const std::string& genre, const int duration)
{
	return m_controller->isMovieUnique(title, language, genre, duration);
}
/*
 * Function: viewSeatLayout
 * Description: Displays the seating grid of a given screen.
 * Parameters:
 *    Screen* - Target screen
 * Returns:
 *    None
 */
void UserInterface::viewSeatLayout(const Screen* screen)
{
	const std::vector<std::vector<Seat*>>& seatGrid = m_controller->getSeatLayout(screen);
	cout << "<------ Seat Grid ------>" << endl << endl;
	for (std::vector<std::vector<Seat*>>::const_iterator iteratorOne = seatGrid.begin(); iteratorOne != seatGrid.end(); ++iteratorOne)
	{
		for (std::vector<Seat*>::const_iterator iteratorTwo = (*iteratorOne).begin(); iteratorTwo != (*iteratorOne).end(); ++iteratorTwo)
		{
			if (!(*iteratorTwo))
			{
				cout << (*iteratorTwo)->getSeatId() << "-[NA]" << " ";
			}
			if ((*iteratorTwo)->getSeatStatus() == Enums::SeatStatus::AVAILABLE)
			{
				cout << (*iteratorTwo)->getSeatId() << "-[A]" << " ";
			}
			else if ((*iteratorTwo)->getSeatStatus() == Enums::SeatStatus::BOOKED)
			{
				cout << (*iteratorTwo)->getSeatId() << "-[B]" << " ";
			}
			else if ((*iteratorTwo)->getSeatStatus() == Enums::SeatStatus::RESERVED)
			{
				cout << (*iteratorTwo)->getSeatId() << "-[R]" << " ";
			}
			else if ((*iteratorTwo)->getSeatStatus() == Enums::SeatStatus::BLOCKED)
			{
				cout << (*iteratorTwo)->getSeatId() << "-[D]" << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
	cout << "[A] - Available  [B] - Booked  [R] - Reserved  [D] - Blocked" << endl;
}

/*
 * Function: UserInterface::getMoviesByTitleInput
 * Description: Prompts the user to enter a movie title and retrieves all movies
 *              matching that title from the Controller. Acts as a user input
 *              handler for initiating movie search operations.
 * Parameters: None
 * Returns:
 *    A vector of constant Movie pointers representing the movies that match
 *    the entered title. Returns an empty vector if no movies are found.
 */
const std::vector<const Movie*> UserInterface::getMoviesByTitleInput()
{
	std::string title;
	cout << "\nEnter the movie title: ";
	util::readValue(title);
	return m_controller->searchMovieByTitle(title);
}

/*
 * Function: UserInterface::validateMovieIdInput
 * Description: Prompts the user to enter a movie ID and validates it against
 *              the list of available movie IDs derived from the provided movies.
 *              Ensures that the entered ID corresponds to a valid movie in the list.
 * Parameters:
 *    movies  - A vector of Movie pointers representing the movies to validate against.
 *    movieId - Reference string to store the user-entered movie ID.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the entered movie ID is valid.
 *    Enums::ProcessStatus::FAILED if the entered movie ID is invalid.
 */
Enums::ProcessStatus UserInterface::validateMovieIdInput(const std::vector<const Movie*>& movies, std::string& movieId)
{
	const vector<string> movieIdList = getMovieIdFromList(movies);
	cout << "\nEnter the Movie ID: ";
	util::readValue(movieId);
	Enums::ProcessStatus status = checkMovieIdIsValid(movieId, movieIdList);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		return Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: UserInterface::editMovieDetails
 * Description: Provides an interactive menu-driven workflow for editing movie details.
 *              Allows the user to update attributes such as title, language, genre,
 *              and duration for a selected movie. Continuously prompts the user until
 *              they choose to exit by entering 0.
 * Parameters:
 *    movieId      - Unique identifier of the movie to be edited.
 *    currentMovie - Pointer to the current Movie object being updated.
 * Returns: None
 */
void UserInterface::editMovieDetails(const std::string& movieId, const Movie* currentMovie)
{
	int choice = 1;
	std::string title, genre, language;
	int duration;
	while (choice != 0)
	{
		displayEditMovieMenu();
		util::readValue(choice);
		switch (choice)
		{
		case 1:
			cout << "\nEnter the new Title: ";
			util::readValue(title);
			changeMovieTitle(movieId, title, currentMovie);
			break;
		case 2:
			cout << "\nEnter the new Language: ";
			util::readValue(language);
			changeMovieLanguage(movieId, language, currentMovie);
			break;
		case 3:
			cout << "\nEnter the new Genre: ";
			util::readValue(genre);
			changeMovieGenre(movieId, genre, currentMovie);
			break;
		case 4:
			cout << "\nEnter the new Duration: ";
			util::readValue(duration);
			changeMovieDuration(movieId, duration, currentMovie);
			break;
		}
	}
}

/*
 * Function: UserInterface::updateMovie
 * Description: Allows the user to update details of an existing movie by searching with its title,
 *              validating the movie ID, and presenting an edit menu for modifying attributes.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::updateMovie()
{
	const std::vector<const Movie*> movies = getMoviesByTitleInput();
	if (movies.empty())
	{
		cout << "\nNo movies found with given title.";
		return;
	}
	displayMovie(movies);
	std::string movieId;
	if (validateMovieIdInput(movies, movieId) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nEnter valid Movie Id from list!";
		return;
	}
	displayCurrentMovieDetails(movieId, movies);
	const Movie* currentMovie = getCurrentMovie(movieId, movies);
	editMovieDetails(movieId, currentMovie);
}

/*
 * Function: UserInterface::getMovieIdFromList
 * Description: Extracts and returns a list of movie IDs from the given movie collection.
 * Parameters:
 *    movies - Vector of constant Movie pointers
 * Returns:
 *    A vector of strings containing movie IDs
 */
const std::vector<std::string> UserInterface::getMovieIdFromList(const std::vector<const Movie*>& movies)
{
	vector<string> movieIds;
	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		const Movie* movie = *iterator;
		if (movie)
		{
			movieIds.push_back(movie->getMovieId());
		}
	}
	return movieIds;
}

/*
 * Function: UserInterface::checkMovieIdIsValid
 * Description: Validates whether the provided movie ID exists within the given list of IDs.
 * Parameters:
 *    movieId     - Movie ID to validate
 *    movieIdList - Vector of valid movie IDs
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the ID is valid,
 *    Enums::ProcessStatus::FAILED otherwise
 */
Enums::ProcessStatus UserInterface::checkMovieIdIsValid(const string& movieId, const std::vector<string>& movieIdList)
{
	bool flag = false;
	for (string id : movieIdList)
	{
		if (id == movieId)
		{
			flag = true;
		}
	}
	if (flag)
	{
		return Enums::ProcessStatus::SUCCESS;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: UserInterface::displayCurrentMovieDetails
 * Description: Displays the details of a specific movie identified by its ID.
 * Parameters:
 *    movieId - Movie ID to display
 *    movies  - Vector of constant Movie pointers
 * Returns:
 *    None
 */
void UserInterface::displayCurrentMovieDetails(const string& movieId, const std::vector<const Movie*>& movies)
{
	cout << "\n-------------------------------------------------------------\n";
	cout << left << setw(10) << "ID"
		<< setw(10) << "Title"
		<< setw(10) << "Language"
		<< setw(10) << "Genre"
		<< setw(10) << "Duration" << endl;
	cout << "-------------------------------------------------------------\n";
	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((*iterator)->getMovieId() == movieId)
		{
			cout << left << setw(10) << (*iterator)->getMovieId()
				<< setw(10) << (*iterator)->getTitle()
				<< setw(10) << (*iterator)->getLanguage()
				<< setw(10) << (*iterator)->getGenre()
				<< setw(10) << (*iterator)->getDuration()
				<< endl;
		}
	}
}

/*
 * Function: UserInterface::getCurrentMovie
 * Description: Retrieves a specific movie object from the list based on its ID.
 * Parameters:
 *    movieId - Movie ID to search for
 *    movies  - Vector of constant Movie pointers
 * Returns:
 *    Pointer to the Movie object if found, otherwise nullptr
 */
const Movie* UserInterface::getCurrentMovie(const string& movieId, const std::vector<const Movie*>& movies)
{
	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if ((*iterator)->getMovieId() == movieId)
		{
			return *iterator;
		}
	}
	return nullptr;
}

/*
 * Function: UserInterface::displayEditMovieMenu
 * Description: Displays the edit menu options for modifying movie details.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::displayEditMovieMenu()
{
	cout << "\n\n1.Title";
	cout << "\n2.Language";
	cout << "\n3.Genre";
	cout << "\n4.Duration";
	cout << "\n0.Exit";
	cout << "\nEnter which details want to edit: ";
}

/*
 * Function: UserInterface::changeMovieTitle
 * Description: Updates the title of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    title       - New title to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieTitle(const string& movieId, const string& title, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(title, currentMovie->getLanguage(), currentMovie->getGenre(), currentMovie->getDuration()))
	{
		if (m_controller->setMovieTitleByID(movieId, title) == Enums::ProcessStatus::SUCCESS)
		{
			cout << "\nTitle has been updated!.";
		}
		else
		{
			cout << "\nTitle can't been Update!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: UserInterface::changeMovieLanguage
 * Description: Updates the language of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    language    - New language to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieLanguage(const string& movieId, const string& language, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), language, currentMovie->getGenre(), currentMovie->getDuration()))
	{
		if ((m_controller->setMovieLanguageByID(movieId, language) == Enums::ProcessStatus::SUCCESS))
		{
			cout << "\nLanguage has been updated!.";
		}
		else
		{
			cout << "\nLanguage can't been update!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: UserInterface::viewTheatreDetails
 * Description: Retrieves and displays details of theatres owned by the current user.
 *              Queries the Controller for theatres associated with the current owner
 *              and presents their information if available. If no theatres are found,
 *              informs the user accordingly.
 * Parameters: None
 * Returns: None
 */
void UserInterface::viewTheatreDetails()
{
	const std::vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
}

/*
 * Function: UserInterface::displayTheatreDetails
 * Description: Displays detailed information about a list of theatres in a
 *              formatted tabular view. Outputs theatre attributes such as
 *              ID, name, city, address, contact phone, and current status.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres
 *               whose details are to be displayed.
 * Returns: None
 */
void UserInterface::displayTheatreDetails(const std::vector<const Theatre*>& theatres)
{
	cout << "\n--------------------------------------------------------------------------------------------------\n";

	cout << left
		<< setw(15) << "ID"
		<< setw(20) << "Name"
		<< setw(15) << "City"
		<< setw(25) << "Address"
		<< setw(15) << "Contact"
		<< setw(15) << "Status"
		<< endl;
	cout << "--------------------------------------------------------------------------------------------------\n";
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if (*iterator)
		{
			cout << left
				<< setw(15) << (*iterator)->getTheatreId()
				<< setw(20) << (*iterator)->getName()
				<< setw(15) << (*iterator)->getCity()
				<< setw(25) << (*iterator)->getAddress()
				<< setw(15) << (*iterator)->getTheatrePhoneNumber()
				<< setw(15) << Enums::getTheatreStatusString((*iterator)->getStatus())
				<< endl;
		}
	}
}

/*
 * Function: UserInterface::changeMovieGenre
 * Description: Updates the genre of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    genre       - New genre to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieGenre(const string& movieId, const string& genre, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), currentMovie->getLanguage(), genre, currentMovie->getDuration()))
	{
		if ((m_controller->setMovieGenreByID(movieId, genre) == Enums::ProcessStatus::SUCCESS))
		{
			cout << "\nGenre has been updated!.";
		}
		else
		{
			cout << "\nGenre can't updated!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: UserInterface::changeMovieDuration
 * Description: Updates the duration of a movie after validating uniqueness.
 * Parameters:
 *    movieId     - Unique identifier of the movie
 *    duration    - New duration (in minutes) to set
 *    currentMovie - Pointer to the current Movie object
 * Returns:
 *    None
 */
void UserInterface::changeMovieDuration(const string& movieId, int& duration, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), currentMovie->getLanguage(), currentMovie->getGenre(), duration))
	{
		if ((m_controller->setMovieDurationByID(movieId, duration) == Enums::ProcessStatus::SUCCESS))
		{
			cout << "\nDuration has been updated!.";
		}
		else
		{
			cout << "\nDuration can't updated!.";
		}
	}
	else
	{
		cout << "\nMovie already exist!" << endl;
		return;
	}
}

/*
 * Function: UserInterface::addMovie
 * Description: Collects movie details from the user, validates uniqueness, and adds the movie to the system.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::addMovie()
{
	string title, language, genre;
	int duration;
	cout << "\nEnter the Movie Title: ";
	util::readValue(title);
	cout << "\nLanguage             : ";
	util::readValue(language);
	cout << "\nGenre                :";
	util::readValue(genre);
	cout << "\nDuration(in minutes) :";
	util::readValue(duration);
	util::isMovieDurationValid(duration);
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(title, language, genre, duration))
	{
		if (m_controller->addMovie(title, language, genre, duration) == Enums::ProcessStatus::SUCCESS)
		{
			cout << "Movie Successfully Added";
		}
		else
		{
			cout << "Movie could not be added!" << endl;
		}
	}
	else
	{
		cout << "\nThe movie already exists!. Please try another. \n";
	}
}

/*
 * Function: UserInterface::reactivateUser
 * Description: Displays all inactive users, prompts the Admin to enter a User ID,
 *              and to activate the selected user. Provides feedback on
 *              whether the activation was successful or failed.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::reactivateUser()
{
	string userId;
	viewInactiveUsers();
	cout << "Enter the User ID: ";
	util::readValue(userId);
	Enums::ProcessStatus result = m_controller->reactivateUser(userId);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		cout << "USER " << userId << " activated successfully" << endl;
	}
	else
	{
		cout << "Invalid User Id, Try again!" << endl;
	}
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::viewLogsByType
 * Description: Displays logs filtered by type. Prompts the user to select a log type
 *              and retrieves matching logs from the controller. Each log entry is
 *              printed to the console.
 * Parameters: None
 * Returns: None
 */
void UserInterface::viewLogsByType()
{
	int choice;
	logsTypeMenu();
	util::readValue(choice);
	std::vector<const Log*> logs;
	switch (choice)
	{
	case 1:
		logs = m_controller->getLogsByType(Enums::LogType::SYSTEM_ACTIVITY);
		break;
	case 2:
		logs = m_controller->getLogsByType(Enums::LogType::ERROR);
		break;
	case 3:
		logs = m_controller->getLogsByType(Enums::LogType::UNKNOWN);
		break;
	default:
		cout << "Invalid choice. Please try again!" << endl;
		util::pressEnter();
		util::clear();
		break;
	}
	util::clear();
	if (logs.empty())
	{
		cout << "No logs available!" << endl;
	}
	for (std::vector<const Log*>::const_iterator iterator = logs.begin(); iterator != logs.end(); ++iterator)
	{
		cout << (*iterator)->toString() << endl;
	}
	util::pressEnter();
}

/*
 * Function: UserInterface::logsTypeMenu
 * Description: Displays the menu of available log types for user selection.
 * Parameters: None
 * Returns: None
 */
void UserInterface::logsTypeMenu()
{
	util::clear();
	cout << "----------------Log Types Menu---------------" << endl;
	cout << "1. " << Enums::getLogTypeString(Enums::LogType::SYSTEM_ACTIVITY) << endl;
	cout << "2. " << Enums::getLogTypeString(Enums::LogType::ERROR) << endl;
	cout << "3. " << Enums::getLogTypeString(Enums::LogType::UNKNOWN) << endl;
	cout << "Select a type: ";
}

/*
 * Function: UserInterface::viewAllUsers
 * Description: Displays all registered users on the console.
 *              Shows user details including User ID, Name, Email, Password,
 *              Role, and Status.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::viewAllUsers()
{
	const vector<const User*>& users = m_controller->getActiveUsers();
	if (users.empty())
	{
		cout << "No users found!" << endl;
	}
	else
	{
		std::cout << std::left
			<< std::setw(10) << "Index"
			<< std::setw(10) << "User ID"
			<< std::setw(20) << "Name"
			<< std::setw(25) << "Email"
			<< std::setw(15) << "Password"
			<< std::setw(15) << "Role"
			<< std::endl;
		std::cout << std::string(100, '-') << std::endl;
		for (vector<const User*>::const_iterator iterator = users.begin(); iterator != users.end(); ++iterator)
		{
			std::cout << std::left
				<< std::setw(10) << (*iterator)->getUserId()
				<< std::setw(20) << (*iterator)->getUserName()
				<< std::setw(25) << (*iterator)->getEmail()
				<< std::setw(15) << (*iterator)->getPassword()
				<< std::setw(15) << Enums::getUserTypeString((*iterator)->getUserType())
				<< std::endl;
		}
	}
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::viewProfile
 * Description: Retrieves the currently authenticated user from the Controller
 *              and displays their profile details.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::viewProfile()
{
	const User* const currentUser = m_controller->getAuthenticatedUser();
	cout << "User Id: " << currentUser->getUserId() << endl;
	cout << "Name : " << currentUser->getUserName() << endl;
	cout << "Email: " << currentUser->getEmail() << endl;
	cout << "Phone Number: " << currentUser->getUserId() << endl;
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::changePassword
 * Description: Prompts the currently authenticated user to change their password.
 *              Requests the current password and a new password from the user,
 *              validates the new password, and pass the change request to
 *              the Controller. Provides feedback on whether the password change
 *              was successful or failed due to a mismatch.
 * Parameters:
 *    currentPassword (string) - The current password entered by the user
 *    newPassword (string)    - The new password entered by the user
 * Returns:
 *    None
 */
void UserInterface::changePassword()
{
	string currentPassword, newPassword;
	cout << "Enter the current password: ";
	util::readValue(currentPassword);
	cout << "Enter the new Password: ";
	util::readValue(newPassword);
	util::isPasswordValid(newPassword);
	Enums::ProcessStatus result = m_controller->changePassword(currentPassword, newPassword);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Password changed successfully." << endl;
	}
	else
	{
		cout << "Password does not match." << endl;
	}
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::viewUserStatus
 * Description: Prompts the user to enter a User ID and retrieves the status
 *              of the specified user from the Controller. Displays whether
 *              the user is Active, Inactive, or not found.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::viewUserStatus()
{
	string userId;
	cout << "Enter User ID: ";
	util::readValue(userId);
	Enums::UserStatus status = m_controller->getUserStatus(userId);
	if (status == Enums::UserStatus::ACTIVE)
	{
		cout << "User is Active" << endl;
	}
	else if (status == Enums::UserStatus::INACTIVE)
	{
		cout << "User is Inactive" << endl;
	}
	else
	{
		cout << "User not found!" << endl;
	}
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::displayAllMovies
 * Description: Retrieves and displays all active movies in the system. If no active movies exist,
 *              informs the user accordingly.
 * Parameters:
 *    None
 * Returns:
 *    None
 */
void UserInterface::displayAllMovies()
{
	const std::vector<const Movie*> movies = m_controller->getAllActiveMovies();
	if (!(movies.empty()))
	{
		displayMovie(movies);
	}
	else
	{
		cout << "\nNo current movies!.";
	}
}

/*
 * Function: UserInterface::DeactivateMovie
 * Description: Allows the user to deactivate a movie in the system by searching with its title,
 *              validating the movie ID, and updating its status to INACTIVE through the Controller.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::deactivateMovie()
{
	const std::vector<const Movie*> movies = m_controller->getAllInactiveMovies();
	if (movies.empty())
	{
		return;
	}
	displayMovie(movies);
	string title, movieId;
	cout << "\nEnter the movie title: ";
	util::readValue(title);
	if (!movies.empty())
	{
		const vector<string> movieIdList = getMovieIdFromList(movies);
		cout << "\nEnter the Movie ID: ";
		util::readValue(movieId);
		if (checkMovieIdIsValid(movieId, movieIdList) == Enums::ProcessStatus::SUCCESS)
		{
			if (m_controller->setMovieDeactivate(movieId) == Enums::ProcessStatus::SUCCESS)
			{
				cout << "\nMovie status has changed to Deactivate!.";
			}
			else
			{
				cout << "\nCan't change the movie status!.";
			}
		}
		else
		{
			cout << "\nEnter valid Movie Id from list!";
		}
	}
	else
	{
		cout << "\nNo movies with " << title << " name!.";
	}
}

/*
 * Function: UserInterface::activateMovie
 * Description: Allows the user to activate a movie in the system by searching with its title,
 *              validating the movie ID, and updating its status to ACTIVE through the Controller.
 * Parameters:
 *    None (reads input directly from the user)
 * Returns:
 *    None
 */
void UserInterface::activateMovie()
{
	if (!displayAllInactiveMovies())
	{
		return;
	}
	string title, movieId;
	cout << "\nEnter the movie title: ";
	util::readValue(title);
	const std::vector<const Movie*> movies = m_controller->searchDeactivatedMovieByTitle(title);
	if (!movies.empty())
	{
		displayMovie(movies);
		const vector<string> movieIdList = getMovieIdFromList(movies);
		cout << "\nEnter the Movie ID: ";
		util::readValue(movieId);
		if (checkMovieIdIsValid(movieId, movieIdList) == Enums::ProcessStatus::SUCCESS)
		{
			if (m_controller->setMovieActivate(movieId) == Enums::ProcessStatus::SUCCESS)
			{
				cout << "\nMovie status has changed to Activate!.";
			}
			else
			{
				cout << "\nCan't change the movie status!.";
			}
		}
		else
		{
			cout << "\nEnter valid Movie Id from list!";
		}
	}
	else
	{
		cout << "\nNo movies with " << title << " name!.";
	}
}

/*
 * Function: UserInterface::searchMovie
 * Description: Allows the user to search for movies by title. Prompts the user
 *              to enter a movie title, retrieves matching movies from the
 *              Controller, and displays the results if found.
 * Parameters: None
 * Returns: None
 */
void UserInterface::searchMovie()
{
	std::string title;
	cout << "\nEnter the movie title: ";
	util::readValue(title);
	const std::vector<const Movie*> movies = m_controller->searchMovieByTitle(title);
	if (!movies.empty())
	{
		displayMovie(movies);
	}
	else
	{
		cout << "No movie exists with name " << title << endl;
	}
}

/*
 * Function: UserInterface::displayAllInactiveMovies
 * Description: Retrieves and displays all inactive movies in the system. If no inactive movies exist,
 *              informs the user accordingly.
 * Parameters:
 *    None
 * Returns:
 *    bool
 */
bool UserInterface::displayAllInactiveMovies()
{
	const std::vector<const Movie*> movies = m_controller->getAllInactiveMovies();
	if (movies.empty())
	{
		cout << "\nNo inactive movies!.";
		return false;
	}
	displayMovie(movies);
	return true;
}
/*
 * Function: deactivateSeat
 * Description: Deactivates a specific seat in the given screen and shows result.
 * Parameters:
 *    Screen* - Target screen
 *    seatId (string) - Identifier of the seat
 * Returns:
 *    None
 */
void UserInterface::deactivateSeat(Screen* screen, const std::string& seatId)
{
	Enums::ProcessStatus status = m_controller->deactivateSeat(screen, seatId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Seat Number " << seatId << " Deactivated!" << endl;
	}
	else
	{
		cout << "Failure! Seat could not be deactivated." << endl;
	}
}

/*
 * Function: reactivateSeat
 * Description: Reactivates a specific seat in the given screen and shows result.
 * Parameters:
 *    screen - Target screen
 *    seatId - Identifier of the seat
 * Returns:
 *    None
 */
void UserInterface::reactivateSeat(Screen* screen, const std::string& seatId)
{
	Enums::ProcessStatus status = m_controller->reactivateSeat(screen, seatId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Seat Number " << seatId << " Reactivated!" << endl;
	}
	else
	{
		cout << "Failure! Seat could not be Reactivated." << endl;
	}
}

/*
* Function Name : viewShowSeatLayout
* Description   : Displays the seat layout for a given show.
*                 Iterates through the 2D seat layout matrix retrieved from the controller
*                 and prints each seat. Also displays the seat status codes
*				  (Available, Booked, Reserved, Blocked, Invalid).
* Parameters    :
*                  show - Pointer to the Show object whose seat layout is to be displayed
* Return Type   : void
*/
void UserInterface::viewShowSeatLayout(const Show* show)
{
	const std::vector<std::vector<std::string>> layout = m_controller->viewShowSeatLayout(show);
	for (std::vector<std::vector<std::string>>::const_iterator rowIterator = layout.begin(); rowIterator != layout.end(); ++rowIterator)
	{
		for (std::vector<std::string>::const_iterator columnIterator = (*rowIterator).begin(); columnIterator != (*rowIterator).end(); ++columnIterator)
		{
			std::cout << *columnIterator << "\t";
		}
		std::cout << std::endl;
	}
	cout << endl;
	cout << "[A] - Available  [B] - Booked  [R] - Reserved  [D] - Blocked  [NA] - Invalid Seat" << endl;
}

/*
* Function Name : addScreen
* Description   : Adds a new screen to the theatre.
* Parameters    :
*                  theatre     - Theatre where the screen is added
*                  name        - Name of the screen
*                  seatRows    - Number of seat rows
*                  seatColumns - Number of seat columns
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::addScreen(const std::string& theatreId, const std::string& name, int seatRows, int seatColumns, double seatAmount)
{
	Enums::ProcessStatus status = m_controller->addScreen(theatreId, name, seatRows, seatColumns, seatAmount);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Name : " << name << " Added Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not add screen." << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Theatre with ID : " << theatreId << " not found" << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with name " << name << " already exists." << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : updateScreenName
* Description   : Updates the name of a screen.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
*                  name     - New screen name
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::updateScreenName(const std::string& theatreId, const std::string& screenId, const std::string& name)
{
	Enums::ProcessStatus status = m_controller->updateScreenName(theatreId, screenId, name);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Name : " << name << " Updated Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not find theatre with Id : " << theatreId << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Screen with ID : " << screenId << " not found in Theatre " << theatreId << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with name " << name << " already exists in Theatre " << theatreId << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : deactivateScreen
* Description   : Deactivates a screen.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::deactivateScreen(const std::string& theatreId, const std::string& screenId)
{
	Enums::ProcessStatus status = m_controller->deactivateScreen(theatreId, screenId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Id : " << screenId << " deactivated Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not deactivate screen with Id " << screenId << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Screen with ID : " << screenId << " not found." << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with ID " << screenId << " already been deactivated." << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : reactivateScreen
* Description   : Reactivates an inactive screen.
* Parameters    :
*                  theatre - Theatre containing the screen
*                  screenId - ID of the screen
* Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::reactivateScreen(const std::string& theatreId, const std::string& screenId)
{
	Enums::ProcessStatus status = m_controller->reactivateScreen(theatreId, screenId);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Screen with Id : " << screenId << " reactivated Successfully." << endl;
		return Enums::ProcessStatus::SUCCESS;
	}
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Failed! Could not reactivate screen with Id " << screenId << endl;
	}
	if (status == Enums::ProcessStatus::NOT_FOUND)
	{
		cout << "Screen with ID : " << screenId << " not found." << endl;
	}
	if (status == Enums::ProcessStatus::ALREADY_EXISTS)
	{
		cout << "Screen with ID " << screenId << " is active." << endl;
	}
	return Enums::ProcessStatus::FAILED;
}

/*
* Function Name : viewTheatreScreens
* Description   : Displays all screens available in the theatre.
* Parameters    :
*                  theatre - Theatre whose screens are viewed
* Return Type   : void
*/
void UserInterface::viewTheatreScreens(const std::string& theatreId)
{
	const std::vector<const Screen*> screens = m_controller->viewTheatreScreens(theatreId);
	if (screens.empty())
	{
		cout << "No screens available." << endl;
		return;
	}
	cout << endl;
	cout << std::left
		<< std::setw(15) << "Screen ID"
		<< std::setw(25) << "Screen Name";
	if (m_controller->getAuthenticatedUserType() == Enums::UserType::ADMIN || m_controller->getAuthenticatedUserType() == Enums::UserType::THEATRE_OWNER)
	{
		cout << std::setw(25) << "Status";
	}
	cout << endl;
	cout << std::string(65, '-') << endl;
	for (std::vector<const Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
	{
		cout << std::left
			<< std::setw(15) << (*iterator)->getScreenId()
			<< std::setw(25) << (*iterator)->getName();
		if (m_controller->getAuthenticatedUserType() == Enums::UserType::ADMIN || m_controller->getAuthenticatedUserType() == Enums::UserType::THEATRE_OWNER)
		{
			cout << std::setw(25) << Enums::getScreenStatusString((*iterator)->getScreenStatus());
		}
		cout << endl;
	}
}

/*
 * Function: UserInterface::displayMovieDetails
 * Description: Displays detailed information about a list of movies in a
 *              formatted tabular view. Outputs movie attributes such as
 *              ID, title, language, genre, duration, and status.
 * Parameters:
 *    movies - A vector of Movie pointers representing the movies
 *             whose details are to be displayed.
 * Returns: None
 */
void UserInterface::displayMovieDetails(const std::vector<const Movie*>& movies)
{
	cout << "\n--------------------------------------------------------------------------------------------------\n";

	cout << left
		<< setw(15) << "ID"
		<< setw(20) << "Title"
		<< setw(15) << "Language"
		<< setw(25) << "Genre"
		<< setw(15) << "Duration"
		<< setw(15) << "Status"
		<< endl;

	cout << "--------------------------------------------------------------------------------------------------\n";

	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if (*iterator)
		{
			cout << left
				<< setw(15) << (*iterator)->getMovieId()
				<< setw(20) << (*iterator)->getTitle()
				<< setw(15) << (*iterator)->getLanguage()
				<< setw(25) << (*iterator)->getGenre()
				<< setw(15) << (*iterator)->getDuration()
				<< setw(15) << Enums::getMovieStatusString((*iterator)->getStatus())
				<< endl;
		}
	}
}

/*
 * Function: UserInterface::searchTheatre
 * Description: Allows the user to search for theatres by name. Prompts the user
 *              to enter a theatre name, retrieves matching theatres from the
 *              Controller, and displays results based on the authenticated
 *              user's role. Admin users see detailed theatre information, while
 *              regular users see a simplified view.
 * Parameters: None
 * Returns: None
 */
void UserInterface::searchTheatre()
{
	std::string theatreName;
	cout << "Enter theatre name: ";
	util::readValue(theatreName);
	const std::vector<const Theatre*> theatres = m_controller->searchTheatreByName(theatreName);
	if (theatres.empty())
	{
		cout << "\nNo theatres found";
		return;
	}
	const User* authenticatedUser = m_controller->getAuthenticatedUser();
	if (authenticatedUser->getUserType() == Enums::UserType::ADMIN)
	{
		displayTheatresForAdmin(theatres);
	}
	else
	{
		displayTheatresForUsers(theatres);
	}
}

/*
 * Function: UserInterface::displayTheatresForAdmin
 * Description: Displays detailed information about a list of theatres in a
 *              formatted tabular view specifically for administrators. Outputs
 *              theatre attributes such as ID, name, city, address, and contact
 *              phone number in a structured layout for easy review.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres
 *               whose details are to be displayed for the admin user.
 * Returns: None
 */
void UserInterface::displayTheatresForAdmin(const std::vector<const Theatre*>& theatres)
{
	cout << "\n---------------------------------------------------------------------------------------\n";
	cout << left
		<< setw(15) << "ID"
		<< setw(20) << "Name"
		<< setw(15) << "City"
		<< setw(25) << "Address"
		<< setw(15) << "Contact"
		<< endl;
	cout << "---------------------------------------------------------------------------------------\n";
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		cout << left
			<< setw(15) << (*iterator)->getTheatreId()
			<< setw(20) << (*iterator)->getName()
			<< setw(15) << (*iterator)->getCity()
			<< setw(25) << (*iterator)->getAddress()
			<< setw(15) << (*iterator)->getTheatrePhoneNumber()
			<< endl;
	}
}

/*
 * Function: UserInterface::displayTheatresForUsers
 * Description: Displays a simplified view of theatre information tailored for
 *              regular users. Outputs theatre attributes such as name, city,
 *              address, and contact phone number in a formatted tabular layout.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres
 *               whose details are to be displayed for user view.
 * Returns: None
 */
void UserInterface::displayTheatresForUsers(const std::vector<const Theatre*>& theatres)
{
	cout << "\n-------------------------------------------------------------\n";
	cout << left
		<< setw(20) << "Name"
		<< setw(15) << "City"
		<< setw(25) << "Address"
		<< setw(15) << "Contact"
		<< endl;
	cout << "-------------------------------------------------------------\n";
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		cout << left
			<< setw(20) << (*iterator)->getName()
			<< setw(15) << (*iterator)->getCity()
			<< setw(25) << (*iterator)->getAddress()
			<< setw(15) << (*iterator)->getTheatrePhoneNumber()
			<< endl;
	}
}

/*
 * Function: UserInterface::listAllTheatres
 * Description: Provides an interactive menu for users to view theatres based on
 *              their status (Active or Inactive). Prompts the user for a choice,
 *              retrieves all theatres from the Controller, and displays them
 *              according to the selected status. Handles invalid input gracefully.
 * Parameters: None
 * Returns: None
 */
void UserInterface::listAllTheatres()
{
	int choice;
	cout << "\n1. Active Theatres";
	cout << "\n2.Inactive Theatres";
	cout << "\n Enter choice: ";
	util::readValue(choice);
	const std::vector<const Theatre*>theatres = m_controller->getAllTheatres();
	if (choice == 1)
	{
		bool isActiveTheatreFound = false;
		for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
		{
			if ((*iterator)->getStatus() == Enums::TheatreStatus::ACTIVE)
			{
				isActiveTheatreFound = true;
				break;
			}
		}
		if (isActiveTheatreFound)
		{
			displayTheatres(theatres, Enums::TheatreStatus::ACTIVE);
		}
		else
		{
			cout << "\nNo active theatres found!";
		}
	}
	else if (choice == 2)
	{
		bool isInactiveTheatreFound = false;
		for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
		{
			if ((*iterator)->getStatus() == Enums::TheatreStatus::INACTIVE)
			{
				isInactiveTheatreFound = true;
				break;
			}
		}
		if (isInactiveTheatreFound)
		{
			displayTheatres(theatres, Enums::TheatreStatus::INACTIVE);
		}
		else
		{
			cout << "\nNo inactive theatres found!";
		}
	}
	else
	{
		cout << "\nInvalid Choice!";
	}
}

/*
 * Function: UserInterface::displayTheatres
 * Description: Displays a formatted tabular view of theatres filtered by their
 *              status (Active or Inactive). Outputs theatre attributes such as
 *              ID, name, city, address, contact phone number, and status string
 *              for each theatre that matches the given status.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres to be displayed.
 *    status   - The TheatreStatus enum value used to filter which theatres are shown.
 * Returns: None
 */
void UserInterface::displayTheatres(const std::vector<const Theatre*>& theatres, Enums::TheatreStatus status)
{
	cout << "\n--------------------------------------------------------------------------------------------------\n";
	cout << left
		<< setw(15) << "ID"
		<< setw(20) << "Name"
		<< setw(15) << "City"
		<< setw(25) << "Address"
		<< setw(15) << "Contact"
		<< setw(15) << "Status"
		<< endl;
	cout << "--------------------------------------------------------------------------------------------------\n";
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if ((*iterator)->getStatus() == status)
		{
			cout << left
				<< setw(15) << (*iterator)->getTheatreId()
				<< setw(20) << (*iterator)->getName()
				<< setw(15) << (*iterator)->getCity()
				<< setw(25) << (*iterator)->getAddress()
				<< setw(15) << (*iterator)->getTheatrePhoneNumber()
				<< setw(15) << Enums::getTheatreStatusString((*iterator)->getStatus())
				<< endl;
		}
	}
}

/*
 * Function: UserInterface::displayMoviesInTheatre
 * Description: Displays movies available in the selected theatre.
 * Parameters:
 *    theatreId (std::string&) - Reference to store selected theatre ID
 * Returns:
 *    void
 */
void UserInterface::displayMoviesInTheatre()
{
	std::string theatreId;
	bool isTheatreIdValid = false;
	const std::vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	displayTheatreDetails(theatres);
	cout << "Enter theatre id of theatre to select: ";
	util::readValue(theatreId);
	const std::vector<std::string> theatreIds = m_controller->getCurrentOwnerTheatreIds();
	for (std::vector<std::string>::const_iterator iterator = theatreIds.begin(); iterator != theatreIds.end(); ++iterator)
	{
		if (theatreId == *iterator)
		{
			isTheatreIdValid = true;
			break;
		}
	}
	if (!isTheatreIdValid)
	{
		cout << "Invalid Theatre id!" << endl;
		util::pressEnter();
		return;
	}
	const std::vector<const Movie*> movies = m_controller->getMoviesFromTheatre(theatreId);
	displayMovieDetails(movies);
}

/*
 * Function: UserInterface::displayMoviesInTheatre
 * Description: Displays movies available in the selected theatre.
 * Parameters:
 *    theatreId (std::string&) - Reference to store selected theatre ID
 * Returns:
 *    void
 */
void UserInterface::displayMoviesInTheatre(std::string& theatreId)
{
	bool isTheatreIdValid = false;
	const std::vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	displayTheatreDetails(theatres);
	cout << "Enter theatre id of theatre to select: ";
	util::readValue(theatreId);
	const std::vector<std::string> theatreIds = m_controller->getCurrentOwnerTheatreIds();
	for (std::vector<std::string>::const_iterator iterator = theatreIds.begin(); iterator != theatreIds.end(); ++iterator)
	{
		if (theatreId == *iterator)
		{
			isTheatreIdValid = true;
			break;
		}
	}
	if (!isTheatreIdValid)
	{
		cout << "Invalid Theatre id!" << endl;
		util::pressEnter();
		return;
	}
	const std::vector<const Movie*> movies = m_controller->getMoviesFromTheatre(theatreId);
	displayMovieDetails(movies);
}

/*
 * Function: UserInterface::validateMovieId
 * Description: Validates whether a given movie ID exists within the list of
 *              available movie IDs. Iterates through the provided IDs and
 *              checks for a match with the input movie ID.
 * Parameters:
 *    movieId  - The movie ID entered by the user to be validated.
 *    movieIds - A vector of strings containing valid movie IDs.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the movie ID is found in the list.
 *    Enums::ProcessStatus::FAILED if the movie ID does not exist.
 */
Enums::ProcessStatus UserInterface::validateMovieId(const std::string& movieId, const std::vector<std::string>& movieIds)
{
	for (std::vector<std::string>::const_iterator iterator = movieIds.begin(); iterator != movieIds.end(); ++iterator)
	{
		if (*iterator == movieId)
		{
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: UserInterface::getMovieIds
 * Description: Extracts and returns the list of movie IDs from a given vector
 *              of Movie pointers. Iterates through the provided movies and
 *              collects their unique identifiers into a string vector.
 * Parameters:
 *    movies - A vector of Movie pointers representing the movies from which
 *             IDs are to be extracted.
 * Returns:
 *    A vector of strings containing the IDs of the provided movies.
 */
const std::vector<std::string> UserInterface::getMovieIds(const std::vector<const Movie*>& movies)
{
	std::vector<std::string> movieIds;
	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		movieIds.push_back((*iterator)->getMovieId());
	}
	return movieIds;
}

/*
 * Function: UserInterface::validateTheatreId
 * Description: Validates whether a given theatre ID exists within the list of
 *              available theatre IDs. Iterates through the provided IDs and
 *              checks for a match with the input theatre ID.
 * Parameters:
 *    theatreId  - The theatre ID entered by the user to be validated.
 *    theatreIds - A vector of strings containing valid theatre IDs.
 * Returns:
 *    Enums::ProcessStatus::SUCCESS if the theatre ID is found in the list.
 *    Enums::ProcessStatus::FAILED if the theatre ID does not exist.
 */
Enums::ProcessStatus UserInterface::validateTheatreId(const std::string& theatreId, const std::vector<std::string>& theatreIds)
{
	for (std::vector<std::string>::const_iterator iterator = theatreIds.begin(); iterator != theatreIds.end(); ++iterator)
	{
		if (*iterator == theatreId)
		{
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: UserInterface::getTheatreIds
 * Description: Extracts and returns the list of theatre IDs from a given vector
 *              of Theatre pointers. Iterates through the provided theatres and
 *              collects their unique identifiers into a string vector.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres from
 *               which IDs are to be extracted.
 * Returns:
 *    A vector of strings containing the IDs of the provided theatres.
 */
const std::vector<std::string> UserInterface::getTheatreIds(const std::vector<const Theatre*>& theatres)
{
	std::vector<std::string> theatreIds;
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		theatreIds.push_back((*iterator)->getTheatreId());
	}
	return theatreIds;
}

/*
 * Function: UserInterface::displayOwnerTheatres
 * Description: Displays a simplified tabular view of theatres owned by the
 *              currently authenticated user. Outputs theatre attributes such
 *              as ID, name, and city for each owned theatre.
 * Parameters:
 *    theatres - A vector of Theatre pointers representing the theatres owned
 *               by the authenticated user.
 * Returns: None
 */
void UserInterface::displayOwnerTheatres(const std::vector<const Theatre*>& theatres)
{
	cout << "\n-------------------------------------------------------------\n";
	cout << left
		<< setw(15) << "ID"
		<< setw(20) << "Name"
		<< setw(15) << "City"
		<< endl;
	cout << "-------------------------------------------------------------\n";
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		cout << left
			<< setw(15) << (*iterator)->getTheatreId()
			<< setw(20) << (*iterator)->getName()
			<< setw(15) << (*iterator)->getCity()
			<< endl;
	}
}

/*
 * Function: UserInterface::displayMovie
 * Description: Displays a formatted list of movies with their details (ID, Title, Language, Genre, Duration).
 * Parameters:
 *    movies - Vector of constant Movie pointers to display
 * Returns:
 *    None
 */
void UserInterface::displayMovie(const std::vector<const Movie*>& movies)
{
	cout << "\n-------------------------------------------------------------\n";
	cout << left << setw(10) << "ID"
		<< setw(10) << "Title"
		<< setw(10) << "Language"
		<< setw(10) << "Genre"
		<< setw(10) << "Duration" << endl;
	cout << "-------------------------------------------------------------\n";
	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		if (*iterator)
		{
			cout << left << setw(10) << (*iterator)->getMovieId()
				<< setw(10) << (*iterator)->getTitle()
				<< setw(10) << (*iterator)->getLanguage()
				<< setw(10) << (*iterator)->getGenre()
				<< setw(10) << (*iterator)->getDuration()
				<< endl;
		}
	}
}

/*
 * Function: UserInterface::addMovieToTheatre
 * Description: Allows a theatre owner to add a movie to one of their theatres.
 *              Prompts the user to select a theatre and a movie from available
 *              lists, validates the entered IDs, and delegates the addition
 *              process to the Controller. Provides feedback on success or
 *              failure (e.g., movie already exists in theatre).
 * Parameters: None
 * Returns: None
 */
void UserInterface::addMovieToTheatre()
{
	std::string theatreId;
	std::string movieId;
	const std::vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (theatres.empty())
	{
		cout << "\nNo theatres found!";
		return;
	}
	displayOwnerTheatres(theatres);
	const std::vector<std::string> theatreIds = getTheatreIds(theatres);
	cout << "\nEnter Theatre ID: ";
	util::readValue(theatreId);
	if (validateTheatreId(theatreId, theatreIds) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nInvalid Theatre ID!";
		return;
	}
	const std::vector<const Movie*> movies = m_controller->getAllActiveMovies();
	if (movies.empty())
	{
		cout << "\nNo active movies found!";
		return;
	}
	displayMovie(movies);
	const std::vector<std::string> movieIds = getMovieIds(movies);
	cout << "\nEnter Movie ID: ";
	util::readValue(movieId);
	if (validateMovieId(movieId, movieIds) == Enums::ProcessStatus::FAILED)
	{
		cout << "\nInvalid Movie ID!";
		return;
	}
	if (m_controller->addMovieToTheatre(theatreId, movieId) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "\nMovie successfully added to theatre!";
	}
	else
	{
		cout << "\nMovie already exists in theatre!";
	}
}

/*
 * Function: UserInterface::handleInputTheatreDetails
 * Description: Validates the entered theatre details by checking whether
 *              the theatre already exists in the system.
 * Parameters:
 *    name (const std::string&) - Name of the theatre
 *    city (const std::string&) - City where the theatre is located
 *    address (const std::string&) - Address of the theatre
 *    phoneNumber (const std::string&) - Contact phone number of the theatre
 *    email (const std::string&) - Contact email address of the theatre
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if the theatre details are unique,
 *                           FAILED otherwise
 */
Enums::ProcessStatus UserInterface::handleInputTheatreDetails(const std::string& name, const std::string& city, const std::string& address, const std::string& phoneNumber, const std::string& email)
{
	return m_controller->isTheatreUnique(name, city, address, phoneNumber, email);
}

/*
 * Function: UserInterface::addTheatre
 * Description: Collects theatre details from the user, validates the input,
 *              ensures unique phone number and email, and sends the theatre
 *              registration request to the controller for approval.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::addTheatre()
{
	std::string name, city, address, phoneNumber, email;
	cout << "\nEnter the name is Theatre   : ";
	util::readValue(name);
	cout << "\nEnter the city              : ";
	util::readValue(city);
	cout << "\nEnter the address           : ";
	util::readValue(address);
	cout << "\nEnter the phone number      : ";
	util::readValue(phoneNumber);
	util::isPhoneNumberValid(phoneNumber);
	getUniqueTheatrePhoneNumber(phoneNumber);
	cout << "\nEnter the email             : ";
	util::readValue(email);
	util::isEmailValid(email);
	getUniqueTheatreEmail(email);
	if (handleInputTheatreDetails(name, city, address, phoneNumber, email) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->addTheatre(name, city, address, phoneNumber, email))
		{
			cout << "\nTheatre added successfully.\nYour request has been sent to the admin for approval.\n";
		}
		else
		{
			cout << "Something went wrong!.";
		}
	}
	else
	{
		cout << "\nTheatre already exist!.";
		return;
	}
}

/*
 * Function: UserInterface::getUniqueTheatrePhoneNumber
 * Description: Ensures that the entered theatre phone number is unique
 *              by repeatedly prompting the user until a unique number is entered.
 * Parameters:
 *    phoneNumber (std::string&) - Reference to the theatre phone number
 * Returns:
 *    void
 */
void UserInterface::getUniqueTheatrePhoneNumber(std::string& phoneNumber)
{
	bool isPhoneNumberUnique = (m_controller->isTheatrePhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS) ? true : false;
	{
		while (!isPhoneNumberUnique)
		{
			cout << "Phone number already exists!. Please enter again: ";
			util::readValue(phoneNumber);
			util::isPhoneNumberValid(phoneNumber);
			if (m_controller->isTheatrePhoneNumberUnique(phoneNumber) == Enums::ProcessStatus::SUCCESS)
			{
				isPhoneNumberUnique = true;
			}
		}
	}
}

/*
 * Function: UserInterface::getUniqueTheatreEmail
 * Description: Ensures that the entered theatre email address is unique
 *              by repeatedly prompting the user until a unique email is entered.
 * Parameters:
 *    email (std::string&) - Reference to the theatre email address
 * Returns:
 *    void
 */
void UserInterface::getUniqueTheatreEmail(std::string& email)
{
	bool isEmailUnique = (m_controller->isTheatreEmailUnique(email) == Enums::ProcessStatus::SUCCESS) ? true : false;
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
 * Function: UserInterface::isValidTheatreID
 * Description: Checks whether the given theatre ID exists in the provided
 *              collection of theatres.
 * Parameters:
 *    theatreId - Unique identifier of the theatre to validate.
 *    theatres  - Vector containing theatre objects to search.
 * Returns:
 *    ProcessStatus::SUCCESS if the theatre ID exists.
 *    ProcessStatus::FAILED if the theatre ID is not found.
 */
Enums::ProcessStatus UserInterface::isValidTheatreID(const std::string& theatreId, const std::vector<const Theatre*>& theatres)
{
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if ((*iterator)->getTheatreId() == theatreId)
		{
			return Enums::ProcessStatus::SUCCESS;
		}
	}
	return Enums::ProcessStatus::FAILED;
}

/*
 * Function: UserInterface::displayEditTheatreMenu
 * Description: Displays the menu options available for editing theatre
 *              details such as name, city, address, phone number, and email.
 * Parameters: None
 * Returns: None
 */
void UserInterface::displayEditTheatreMenu()
{
	cout << "\n\n1.Name";
	cout << "\n2.City";
	cout << "\n3.Address";
	cout << "\n4.Phone Number";
	cout << "\n5.Email";
	cout << "\n0.Exit";
	cout << "\nEnter which details want to edit: ";
}

/*
 * Function: UserInterface::updateTheatre
 * Description: Allows the current theatre owner to update theatre details.
 *              Displays available theatres, validates the selected theatre ID,
 *              and provides options to edit theatre information.
 * Parameters: None
 * Returns: None
 */
void UserInterface::updateTheatre()
{
	std::string theatreId, name, city, address, phoneNumber, email;
	int choice = 1;
	const std::vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		cout << "\nEnter the theatre Id, which you want to edit: ";
		util::readValue(theatreId);
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			const Theatre* theatre = getCurrentTheatreById(theatreId, theatres);
			while (choice != 0)
			{
				displayEditTheatreMenu();
				util::readValue(choice);
				if (choice == 1)
				{
					cout << "\nEnter the new name: ";
					util::readValue(name);
					changeTheatreName(theatreId, name, theatre);
				}
				else if (choice == 2)
				{
					cout << "\nEnter the new city: ";
					util::readValue(city);
					changeTheatreCity(theatreId, city, theatre);
				}
				else if (choice == 3)
				{
					cout << "\nEnter the new address: ";
					util::readValue(address);
					changeTheatreAddress(theatreId, address, theatre);
				}
				else if (choice == 4)
				{
					cout << "\nEnter the new phone number: ";
					util::readValue(phoneNumber);
					util::isPhoneNumberValid(phoneNumber);
					getUniqueTheatrePhoneNumber(phoneNumber);
					changeTheatrePhoneNumber(theatreId, phoneNumber, theatre);
				}
				else if (choice == 5)
				{
					cout << "\nEnter the new email: ";
					util::isEmailValid(email);
					getUniqueTheatreEmail(email);
					changeTheatreEmail(theatreId, email, theatre);
				}
				else
				{
					cout << "\nEnter a valid choice!.";
				}
			}
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "No theatres found has added" << endl;
	}
}

/*
 * Function: UserInterface::changeTheatreName
 * Description: Updates the name of the selected theatre after validating
 *              the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    name      - New theatre name.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreName(const std::string& theatreId, const std::string& name, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(name, theatre->getCity(), theatre->getAddress(), theatre->getTheatrePhoneNumber(), theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreNameById(theatreId, name))
		{
			cout << "Name has been updated!.";
		}
		else
		{
			cout << "\nName can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: UserInterface::changeTheatreCity
 * Description: Updates the city of the selected theatre after validating
 *              the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    city      - New city name.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreCity(const std::string& theatreId, const std::string& city, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), city, theatre->getAddress(), theatre->getTheatrePhoneNumber(), theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreCityById(theatreId, city))
		{
			cout << "\nCity has been updated!.";
		}
		else
		{
			cout << "\nCity can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: UserInterface::changeTheatreAddress
 * Description: Updates the address of the selected theatre after validating
 *              the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    address   - New address of the theatre.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreAddress(const std::string& theatreId, const std::string& address, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), theatre->getCity(), address, theatre->getTheatrePhoneNumber(), theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreAddressById(theatreId, address))
		{
			cout << "\nAddress has been updated!.";
		}
		else
		{
			cout << "\nAddress can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: UserInterface::changeTheatrePhoneNumber
 * Description: Updates the phone number of the selected theatre after
 *              validating the updated theatre details.
 * Parameters:
 *    theatreId   - Unique identifier of the theatre.
 *    phoneNumber - New phone number of the theatre.
 *    theatre     - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatrePhoneNumber(const std::string& theatreId, const std::string& phoneNumber, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), theatre->getCity(), theatre->getAddress(), phoneNumber, theatre->getTheatreEmail()) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatrePhoneNumberById(theatreId, phoneNumber))
		{
			cout << "\nPhone number has been updated!.";
		}
		else
		{
			cout << "\nPhone Number can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: UserInterface::changeTheatreEmail
 * Description: Updates the email address of the selected theatre after
 *              validating the updated theatre details.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    email     - New email address of the theatre.
 *    theatre   - Reference to the current theatre object.
 * Returns: None
 */
void UserInterface::changeTheatreEmail(const std::string& theatreId, const std::string& email, const Theatre*& theatre)
{
	if (handleInputTheatreDetails(theatre->getName(), theatre->getCity(), theatre->getAddress(), theatre->getTheatrePhoneNumber(), email) == Enums::ProcessStatus::SUCCESS)
	{
		if (Enums::ProcessStatus::SUCCESS == m_controller->setTheatreEmailById(theatreId, email))
		{
			cout << "\nEmail has been updated!.";
		}
		else
		{
			cout << "\nEmail can't updated!.";
		}
	}
	else
	{
		cout << "\nThis Theatre already exist!.\n";
		return;
	}
}

/*
 * Function: UserInterface::getCurrentTheatreById
 * Description: Retrieves the theatre object that matches the given
 *              theatre ID from the provided theatre collection.
 * Parameters:
 *    theatreId - Unique identifier of the theatre.
 *    theatres  - Vector containing theatre objects.
 * Returns:
 *    Pointer to the matching Theatre object if found.
 */
const Theatre* UserInterface::getCurrentTheatreById(const std::string& theatreId, const std::vector<const Theatre*>& theatres)
{
	for (std::vector<const Theatre*>::const_iterator iterator = theatres.begin(); iterator != theatres.end(); ++iterator)
	{
		if ((*iterator)->getTheatreId() == theatreId)
		{
			return *iterator;
		}
	}
	return nullptr;
}

/*
 * Function: UserInterface::validateTheatreRequest
 * Description: Allows the administrator to validate pending theatre
 *              requests by approving or rejecting a selected theatre.
 *              Displays all pending theatres, validates the selected
 *              theatre ID, and updates the theatre status based on
 *              the administrator's choice.
 * Parameters: None
 * Returns: None
 */
void UserInterface::validateTheatreRequest()
{
	std::string theatreId;
	int choice;
	const std::vector<const Theatre*> pendingTheatres = m_controller->getPendingTheatres();
	if (!pendingTheatres.empty())
	{
		displayTheatreDetails(pendingTheatres);
		cout << "\nEnter the theatre Id, which you want to validate: ";
		util::readValue(theatreId);
		if (isValidTheatreID(theatreId, pendingTheatres) == Enums::ProcessStatus::SUCCESS)
		{
			displayTheatreValidationMenu();
			util::readValue(choice);
			if (choice == 1)
			{
				setTheatreStatusById(theatreId, Enums::TheatreStatus::ACTIVE);
			}
			else if (choice == 2)
			{
				setTheatreStatusById(theatreId, Enums::TheatreStatus::PENDING);
			}
			else if (choice == 0)
			{
				exit(0);
			}
			else
			{
				cout << "\nEnter a valid choice!.";
			}
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "\nNo pending theatre request!";
	}
}

/*
 * Function: UserInterface::displayTheatreValidationMenu
 * Description: Displays the theatre validation menu with options to
 *              approve, reject, or exit the validation process.
 * Parameters: None
 * Returns: None
 */
void UserInterface::displayTheatreValidationMenu()
{
	cout << "\n1. Approve";
	cout << "\n2. Reject";
	cout << "\n0. Exit";
	cout << "\nEnter the choice: ";
}

/*
 * Function: UserInterface::setTheatreStatusById
 * Description: Updates the status of a theatre identified by the given
 *              theatre ID. Delegates the status update operation to
 *              the controller layer.
 * Parameters:
 *    theatreId    - Unique identifier of the theatre.
 *    theatreStatus - New status to be assigned to the theatre.
 * Returns: None
 */
void UserInterface::setTheatreStatusById(const std::string& theatreId, Enums::TheatreStatus theatreStatus)
{
	if (m_controller->setTheatreStatusById(theatreId, theatreStatus) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "\nTheatre Status has been updated!.";
	}
	else
	{
		cout << "\nSomething went wrong!.";
	}
}

/*
 * Function: UserInterface::deactivateTheatreByOwner
 * Description: Allows the current theatre owner to deactivate one of
 *              their theatres by selecting a valid theatre ID from
 *              the list of owned theatres. Updates the selected
 *              theatre's status to INACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::deactivateTheatreByOwner()
{
	std::string theatreId;
	const std::vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		cout << "\nEnter the theatre Id, which you want to edit: ";
		util::readValue(theatreId);
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::INACTIVE);
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
}

/*
 * Function: UserInterface::deactivateTheatreByAdmin
 * Description: Allows the administrator to deactivate a theatre by
 *              selecting a valid theatre ID from the list of all
 *              available theatres. Updates the selected theatre's
 *              status to INACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::deactivateTheatreByAdmin()
{
	std::string theatreId;
	const std::vector<const Theatre*> theatres = m_controller->listAllTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		cout << "\nEnter the theatre Id, which you want to edit: ";
		util::readValue(theatreId);
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::INACTIVE);
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
}

/*
 * Function: UserInterface::reactivateTheatreByOwner
 * Description: Allows the current theatre owner to deactivate one of
 *              their theatres by selecting a valid theatre ID from
 *              the list of owned theatres. Updates the selected
 *              theatre's status to ACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::reactivateTheatreByOwner()
{
	std::string theatreId;
	const std::vector<const Theatre*> theatres = m_controller->getCurrentOwnerTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		cout << "\nEnter the theatre Id, which you want to edit: ";
		util::readValue(theatreId);
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::PENDING);
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
}

/*
 * Function: UserInterface::reactivateTheatreByAdmin
 * Description: Allows the administrator to reactivate a theatre by
 *              selecting a valid theatre ID from the list of all
 *              available theatres. Updates the selected theatre's
 *              status to ACTIVE.
 * Parameters: None
 * Returns: None
 */
void UserInterface::reactivateTheatreByAdmin()
{
	std::string theatreId;
	const std::vector<const Theatre*> theatres = m_controller->listAllTheatres();
	if (!theatres.empty())
	{
		displayTheatreDetails(theatres);
		cout << "\nEnter the theatre Id, which you want to edit: ";
		util::readValue(theatreId);
		if (isValidTheatreID(theatreId, theatres) == Enums::ProcessStatus::SUCCESS)
		{
			setTheatreStatusById(theatreId, Enums::TheatreStatus::ACTIVE);
		}
		else
		{
			cout << "\nEnter the valid theatre id";
		}
	}
	else
	{
		cout << "No theatres found for current owner" << endl;
	}
}

/*
 * Function: UserInterface::isValidTime
 * Description: Validates whether the given hour and minute represent a valid time.
 * Parameters:
 *    hour (int) - Hour value (0–23)
 *    minute (int) - Minute value (0–59)
 * Returns:
 *    bool - True if valid, false otherwise
 */
bool UserInterface::isValidTime(int hour, int minute)
{
	return (hour >= 0 && hour < 24 && minute >= 0 && minute < 60);
}

/*
 * Function: UserInterface::isValidDate
 * Description: Validates whether the given date is valid, including leap year handling.
 * Parameters:
 *    year (int) - Year value
 *    month (int) - Month value
 *    day (int) - Day value
 * Returns:
 *    bool - True if valid, false otherwise
 */
bool UserInterface::isValidDate(int year, int month, int day)
{
	if (year < 1900 || month < 1 || month > 12 || day < 1)
	{
		return false;
	}
	int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		daysInMonth[1] = 29;
	}
	return day <= daysInMonth[month - 1];
}

/*
 * Function: UserInterface::getValidTime
 * Description: Prompts the user until a valid time is entered.
 * Parameters:
 *    hour (int&) - Reference to store valid hour
 *    minute (int&) - Reference to store valid minute
 * Returns:
 *    void
 */
void UserInterface::getValidTime(int& hour, int& minute)
{
	bool isTimeValid = isValidTime(hour, minute);
	while (!isTimeValid)
	{
		cout << "Please enter a valid time (HH MM) : ";
		util::readValue(hour);
		util::readValue(minute);
		isTimeValid = isValidTime(hour, minute);
	}
}

/*
 * Function: UserInterface::getValidDate
 * Description: Prompts the user until a valid date is entered.
 * Parameters:
 *    year (int&) - Reference to store valid year
 *    month (int&) - Reference to store valid month
 *    day (int&) - Reference to store valid day
 * Returns:
 *    void
 */
void UserInterface::getValidDate(int& year, int& month, int& day)
{
	bool isDateValid = isValidDate(year, month, day);
	while (!isDateValid)
	{
		cout << "Please enter a valid date (YYYY MM DD) : ";
		util::readValue(year);
		util::readValue(month);
		util::readValue(day);
		isDateValid = isValidDate(year, month, day);
	}
}

/*
 * Function: UserInterface::isFutureDateTime
 * Description: Checks whether the given date and time occur in the future.
 * Parameters:
 *    year (int) - Year value
 *    month (int) - Month value
 *    day (int) - Day value
 *    hour (int) - Hour value
 *    minute (int) - Minute value
 * Returns:
 *    bool - True if the datetime is in the future, false otherwise
 */
bool UserInterface::isFutureDateTime(int year, int month, int day, int hour, int minute)
{
	time_t inputTime = util::createTime(year, month, day, hour, minute);
	if (inputTime == -1)
	{
		return false;
	}
	time_t currentTime = time(0);
	return difftime(inputTime, currentTime) > 0;
}

/*
 * Function: UserInterface::addShow
 * Description: Handles user interaction to add a new show, including
 *              input collection, validation, and invoking controller logic.
 * Parameters: None
 * Returns:
 *    void
 */
void UserInterface::addShow()
{
	string showId;
	string movieId, screenId, theatreId;
	int year, month, day;
	int startTimeHour, startTimeMinute;
	displayMoviesInTheatre(theatreId);
	cout << "Enter Movie ID: ";
	util::readValue(movieId);
	Enums::ProcessStatus isMoviePresent = m_controller->isMovieInTheatre(movieId, theatreId);
	if (isMoviePresent == Enums::ProcessStatus::FAILED)
	{
		cout << "Movie is not present in theatre" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	const std::vector<const Screen*> screens = m_controller->getScreensFromTheatre(theatreId);
	if (screens.empty())
	{
		cout << "No screens available to add show to! " << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	if (!getScreenId(screens, screenId))
	{
		cout << "Invalid screen ID!" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	cout << "Enter date (YYYY MM DD): ";
	util::readValue(year);
	util::readValue(month);
	util::readValue(day);
	getValidDate(year, month, day);
	cout << "Enter start time (HH MM): ";
	util::readValue(startTimeHour);
	util::readValue(startTimeMinute);
	getValidTime(startTimeHour, startTimeMinute);
	if (!isFutureDateTime(year, month, day, startTimeHour, startTimeMinute))
	{
		cout << "Cannot add a show with a past time/date!" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	if (m_controller->isShowTimeConflicting(movieId, screenId, year, month, day, startTimeHour, startTimeMinute) == Enums::ProcessStatus::FAILED)
	{
		cout << "Cannot add show as it conflicts with the time of another show!" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	if (m_controller->addShow(movieId, screenId, year, month, day, startTimeHour, startTimeMinute) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Show added successfully!" << endl;
		util::pressEnter();
	}
}

/*
*Function: UserInterface::getScreenId
* Description : Displays available screens and validates the selected screen ID.
* Parameters :
	*screens(const std::vector<const Screen*>&) - List of available screens
	* screenId(std::string&) - Reference to store selected screen ID
	* Returns :
	*bool - True if a valid screen ID is selected, false otherwise
*/
bool UserInterface::getScreenId(const std::vector<const Screen*>& screens, std::string& screenId)
{
	std::vector<std::string> screenIds;
	cout << "\nAvaiable Screens\n-----------------------------" << endl;
	for (std::vector<const Screen*>::const_iterator iterator = screens.begin(); iterator != screens.end(); ++iterator)
	{
		cout << (*iterator)->getScreenId() << "   " << (*iterator)->getName() << endl;
		screenIds.push_back((*iterator)->getScreenId());
	}
	cout << "Enter screen id of screen to add to: ";
	util::readValue(screenId);
	for (std::vector<std::string>::iterator iterator = screenIds.begin(); iterator != screenIds.end(); ++iterator)
	{
		if ((*iterator) == screenId)
		{
			return true;
		}
	}
	return false;
}

/*
*Function: UserInterface::displayActiveShows
* Description : Displays all active shows retrieved from Controller in tabular format.
* Returns :
	*void
*/
void UserInterface::displayActiveShows()
{
	const std::vector<const Show*> shows = m_controller->getActiveShows();
	if (shows.empty())
	{
		cout << "No shows available!" << endl;
		util::pressEnter();
		return;
	}
	displayShowDetails(shows);
}

/*
 * Function: UserInterface::displayAllShows
 * Description: Displays all shows in tabular format for the authenticated theatre owner.
 * Returns:
 *    void
 */
void UserInterface::displayAllShows()
{
	const std::vector<const Show*> shows = m_controller->getAllShows();
	if (shows.empty())
	{
		cout << "No shows available!" << endl;
		util::pressEnter();
		return;
	}
	displayShowDetails(shows);
}

/*
 * Function: UserInterface::displayShowDetails
 * Description: Displays detailed information for a list of shows including theatre, screen, movie, and start time.
 * Parameters:
 *    const std::vector<const Show*> shows - List of shows to display
 * Returns:
 *    void
 */
void UserInterface::displayShowDetails(const std::vector<const Show*> shows)
{
	if (shows.empty())
	{
		cout << "NO SHOWS TO DISPLAY!" << endl;
		return;
	}
	cout << "\n--------------------------------------------------------------------------------------------------\n";
	cout << left
		<< setw(15) << "ID"
		<< setw(20) << "Theatre Name"
		<< setw(15) << "Screen Id"
		<< setw(25) << "Movie Title"
		<< setw(15) << "Show Date and time"
		<< endl;
	cout << "--------------------------------------------------------------------------------------------------\n";
	for (std::vector<const Show*>::const_iterator iterator = shows.begin(); iterator != shows.end(); ++iterator)
	{
		cout << left
			<< setw(15) << (*iterator)->getShowId()
			<< setw(20) << (*iterator)->getScreen()->getTheatre()->getName()
			<< setw(15) << (*iterator)->getScreen()->getScreenId()
			<< setw(25) << (*iterator)->getMovie()->getTitle()
			<< setw(15) << displayTimeAndDate((*iterator)->getStartTime())
			<< endl;
	}
}

/*
 * Function: UserInterface::displayTimeAndDate
 * Description: Formats a time_t value into a human-readable string "YYYY-MM-DD HH:MM:SS".
 * Parameters:
 *    time_t time - The time value to format
 * Returns:
 *    std::string - Formatted date/time string
 */
std::string UserInterface::displayTimeAndDate(time_t time)
{
	std::tm local{};
	localtime_s(&local, &time);
	std::ostringstream outputStream;
	outputStream << (1900 + local.tm_year) << "-"
		<< std::setw(2) << std::setfill('0') << (1 + local.tm_mon) << "-"
		<< std::setw(2) << std::setfill('0') << local.tm_mday << " "
		<< std::setw(2) << std::setfill('0') << local.tm_hour << ":"
		<< std::setw(2) << std::setfill('0') << local.tm_min << ":"
		<< std::setw(2) << std::setfill('0') << local.tm_sec;
	return outputStream.str();
}

/*
 * Function: UserInterface::cancelShow
 * Description: Handles user interaction to cancel a show by validating the show ID,
 *              checking cancellability, and updating the show status.
 * Parameters: None
 * Returns:
 *    void
 */
void UserInterface::cancelShow()
{
	std::string showId;
	displayActiveShows();
	cout << "Enter the show id of show to cancel: ";
	util::readValue(showId);
	const std::vector<std::string> showIds = m_controller->getActiveShowIds();
	bool isShowIdValid = false;
	for (std::vector<std::string>::const_iterator iterator = showIds.begin(); iterator != showIds.end(); ++iterator)
	{
		if (*iterator == showId)
		{
			isShowIdValid = true;
			break;
		}
	}
	if (!isShowIdValid)
	{
		cout << "Show id is not valid!" << endl;
		util::pressEnter();
		return;
	}
	Enums::ProcessStatus isShowCancellable = m_controller->isShowChangable(showId);
	if (isShowCancellable == Enums::ProcessStatus::FAILED)
	{
		cout << "Show is not cancellable" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	Enums::ProcessStatus status = m_controller->setShowStatusById(showId, Enums::ShowStatus::CANCELLED);
	if (status == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Show cancelled successfully" << endl;
		util::pressEnter();
		util::clear();
	}
	else
	{
		cout << "Failed to cancel show" << endl;
		util::pressEnter();
		util::clear();
	}
}

/*
 * Function: UserInterface::viewShowStatus
 * Description: Allows the user to view the status of a specific show by ID.
 * Returns:
 *    void
 */
void UserInterface::viewShowStatus()
{
	std::string showId;
	displayAllShows();
	cout << "Enter the show id of show to see status of: ";
	util::readValue(showId);
	const std::vector<std::string> showIds = m_controller->getAllShowIds();
	bool isShowIdValid = false;
	for (std::vector<std::string>::const_iterator iterator = showIds.begin(); iterator != showIds.end(); ++iterator)
	{
		if (*iterator == showId)
		{
			isShowIdValid = true;
			break;
		}
	}
	if (!isShowIdValid)
	{
		cout << "Show id is not valid!" << endl;
		util::pressEnter();
		return;
	}
	Enums::ShowStatus status = m_controller->getShowStatus(showId);
	cout << " Show Status: " << Enums::getShowStatusString(status);
	util::pressEnter();
	util::clear();
}

/*
 * Function: UserInterface::updateShow
 * Description: Handles user interaction to update a show’s time after validation.
 * Returns:
 *    void
 */
void UserInterface::updateShow()
{
	std::string showId;
	displayAllShows();
	cout << "Enter the show id of show to update: ";
	util::readValue(showId);
	const std::vector<std::string> showIds = m_controller->getAllShowIds();
	bool isShowIdValid = false;
	for (std::vector<std::string>::const_iterator iterator = showIds.begin(); iterator != showIds.end(); ++iterator)
	{
		if (*iterator == showId)
		{
			isShowIdValid = true;
			break;
		}
	}
	if (!isShowIdValid)
	{
		cout << "Show id is not valid!" << endl;
		util::pressEnter();
		return;
	}
	Enums::ProcessStatus isShowUpdatable = m_controller->isShowChangable(showId);
	if (isShowUpdatable == Enums::ProcessStatus::FAILED)
	{
		cout << "Show cannot be updated because it has completed bookings!" << endl;
		util::pressEnter();
		util::clear();
	}
	time_t newTimeAndDate;
	Enums::ProcessStatus status = getNewDateAndTime(newTimeAndDate);
	if (status == Enums::ProcessStatus::FAILED)
	{
		cout << "Invalid time!" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	if (m_controller->isNewShowTimeConflicting(showId, newTimeAndDate) == Enums::ProcessStatus::FAILED)
	{
		cout << "Cannot update show time as it conflicts with the time of another show!" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	if (m_controller->updateShow(newTimeAndDate, showId) == Enums::ProcessStatus::SUCCESS)
	{
		cout << "Show updated successfully!" << endl;
		util::pressEnter();
	}
	else
	{
		cout << "Failed to update show!" << endl;
		util::pressEnter();
	}
}

/*
 * Function: UserInterface::getNewDateAndTime
 * Description: Reads and validates new date/time input from user, ensuring it is in the future.
 * Parameters:
 *    time (time_t&) - Reference to store the constructed time
 * Returns:
 *    Enums::ProcessStatus - SUCCESS if valid, FAILED otherwise
 */
Enums::ProcessStatus UserInterface::getNewDateAndTime(time_t& time)
{
	int year, month, day, startTimeHour, startTimeMinute;
	cout << "Enter date (YYYY MM DD): ";
	util::readValue(year);
	util::readValue(month);
	util::readValue(day);
	getValidDate(year, month, day);
	cout << "Enter start time (HH MM): ";
	util::readValue(startTimeHour);
	util::readValue(startTimeMinute);
	getValidTime(startTimeHour, startTimeMinute);
	if (!isFutureDateTime(year, month, day, startTimeHour, startTimeMinute))
	{
		cout << "Cannot update a show with a past time/date!" << endl;
		util::pressEnter();
		util::clear();
		return Enums::ProcessStatus::FAILED;
	}
	time = util::createTime(year, month, day, startTimeHour, startTimeMinute);
	if (time == -1)
	{
		return Enums::ProcessStatus::FAILED;
	}
	return Enums::ProcessStatus::SUCCESS;
}

/*
 * Function: UserInterface::listShowsForAMovie
 * Description: Displays all active movies, validates user input, and lists shows for the selected movie.
 * Parameters:
 *    None
 * Returns:
 *    void
 */
void UserInterface::listShowsForAMovie()
{
	std::vector<const Movie*> movies = m_controller->getAllActiveMovies();
	std::string movieId;
	displayAllMovies();
	cout << "Enter the id of a movie to search shows for: ";
	util::readValue(movieId);
	if (validateMovieIdInput(movies, movieId) == Enums::ProcessStatus::FAILED)
	{
		cout << "Invalid movie id!" << endl;
		util::pressEnter();
		util::clear();
		return;
	}
	const std::vector<const Show*> shows = m_controller->getShowsForMovie(movieId);
	displayShowDetails(shows);
}

/*
* Function Name : UserInterface::viewActiveTicketDetails
* Description   : Displays all active tickets for the authenticated user.
*                 Active tickets are retrieved from the controller and shown
*                 using the viewTicketDetails helper.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewActiveTicketDetails()
{
	const std::vector<const Ticket*> tickets = m_controller->viewTicketDetails();
	if (tickets.empty())
	{
		cout << "No Tickets Available" << endl;
		return;
	}
	viewTicketDetails(tickets);
}

/*
* Function Name : UserInterface::viewAllTickets
* Description   : Displays all tickets available in the system by retrieving
*                 them from the controller and showing details.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewAllTickets()
{
	const std::vector<const Ticket*> tickets = m_controller->viewAllTickets();
	if (tickets.empty())
	{
		cout << "No Tickets Available" << endl;
		return;
	}
	viewTicketDetails(tickets);
}

/*
* Function Name : UserInterface::viewTicketHistory
* Description   : Displays the complete ticket history for the authenticated user.
*                 Tickets are retrieved from the controller and shown using the helper.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewTicketHistory()
{
	const std::vector<const Ticket*> tickets = m_controller->viewTicketHistory();
	if (tickets.empty())
	{
		cout << "No Tickets Available" << endl;
		return;
	}
	viewTicketDetails(tickets);
}

/*
* Function Name : UserInterface::viewTicketDetails
* Description   : Helper function to display details of tickets including
*                 Ticket ID, Payment ID, Amount, Booking ID, and Ticket Status.
* Parameters    :
*                  tickets - Vector of Ticket pointers to display
* Return Type   : void
*/
void UserInterface::viewTicketDetails(const std::vector<const Ticket*>& tickets)
{
	Enums::UserType type = m_controller->getAuthenticatedUserType();
	std::cout << std::left
		<< std::setw(12) << "Ticket ID";
	if (type == Enums::UserType::ADMIN)
	{
		cout << std::setw(15) << "Customer Name";
	}
	cout << std::setw(12) << "Payment ID"
		<< std::setw(10) << "Amount"
		<< std::setw(12) << "Booking ID"
		<< std::setw(12) << "Status"
		<< std::endl;
	std::cout << std::string(73, '-') << std::endl;
	for (std::vector<const Ticket*>::const_iterator iterator = tickets.begin(); iterator != tickets.end(); ++iterator)
	{
		const Ticket* ticket = *iterator;
		std::cout << std::left << std::setw(12) << ticket->getTicketId();
		if (type == Enums::UserType::ADMIN)
		{
			std::cout << std::setw(15) << ticket->getCustomer()->getUserName();
		}
		else
		{
			std::cout << std::setw(15) << "-";
		}
		std::cout << std::setw(12) << ticket->getPayment()->getPaymentId()
			<< std::setw(10) << ticket->getPayment()->getAmount()
			<< std::setw(12) << ticket->getPayment()->getBooking()->getBookingId()
			<< std::setw(12) << Enums::getTicketStatusString(ticket->getTicketStatus())
			<< std::endl;
	}
}

/*
* Function Name : UserInterface::viewTicketStatus
* Description   : Prompts the user to enter a Ticket ID, retrieves the ticket status
*                 from the controller, and displays a message based on the status.
* Parameters    : None
* Return Type   : void
*/
void UserInterface::viewTicketStatus()
{
	string ticketId;
	cout << "Enter the Ticket ID:";
	util::readValue(ticketId);
	Enums::TicketStatus status = m_controller->viewTicketStatus(ticketId);
	if (status == Enums::TicketStatus::ACTIVE)
	{
		cout << "Ticket is Active." << endl;
	}
	else if (status == Enums::TicketStatus::COMPLETED)
	{
		cout << "Ticket has been expired." << endl;
	}
	else if (status == Enums::TicketStatus::CANCELLED)
	{
		cout << "Ticket has been cancelled." << endl;
	}
	else
	{
		cout << "Ticket with ID : " << ticketId << " not found!" << endl;
	}
}

/*
Function Name : handleCardPayment
Description   : Handles card-based payment input (Credit/Debit).
				Prompts the user for card details with retry until valid input is provided.
Parameters    :
				 type - The payment method (Credit or Debit Card)
Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::handleCardPayment(Enums::PaymentMethod type)
{
	type = (type == Enums::PaymentMethod::CREDIT_CARD) ? Enums::PaymentMethod::CREDIT_CARD : Enums::PaymentMethod::DEBIT_CARD;
	std::string cardNumber, expiry, cvv;
	util::readValueWithRetry(cardNumber, "Enter Card Number: ");
	util::readValueWithRetry(expiry, "Enter Expiry (MM/YY): ");
	util::readValueWithRetry(cvv, "Enter CVV: ");
	while (!util::validateCard(cardNumber, expiry, cvv))
	{
		std::cout << "Error: Invalid card details. Please try again.\n";
		util::readValueWithRetry(cardNumber, "Enter Card Number: ");
		util::readValueWithRetry(expiry, "Enter Expiry (MM/YY): ");
		util::readValueWithRetry(cvv, "Enter CVV: ");
	}
	return Enums::ProcessStatus::SUCCESS;
}

/*
Function Name : handleUPIPayment
Description   : Handles UPI-based payment input.
				Prompts the user for UPI ID with retry until valid input is provided.
Parameters    :
				 type - The payment method (UPI)
Return Type   : Enums::ProcessStatus
*/
Enums::ProcessStatus UserInterface::handleUPIPayment(Enums::PaymentMethod type)
{
	type = Enums::PaymentMethod::UPI;
	std::string upiId;

	util::readValueWithRetry(upiId, "Enter UPI ID: ");
	while (!util::validateUPI(upiId))
	{
		std::cout << "Error: Invalid UPI ID. Please try again.\n";
		util::readValueWithRetry(upiId, "Enter UPI ID: ");
	}

	return Enums::ProcessStatus::SUCCESS;
}

/*
Function Name : displayPaymentOptions
Description   : Displays available payment methods to the user
				and reads their choice.
Parameters    : None
Return Type   : int
*/
int UserInterface::displayPaymentOptions()
{
	std::cout << "Select Payment Method:\n";
	std::cout << "1. Credit Card\n";
	std::cout << "2. Debit Card\n";
	std::cout << "3. UPI\n";
	std::cout << "Enter choice: ";
	int choice;
	util::readValue(choice);
	return choice;
}

/*
 * Function: UserInterface::displayPaymentStatus
 * Description: Displays the details of a payment based on its unique identifier.
 *              Retrieves payment information from the Controller and prints
 *              the payment ID, booking ID, amount, method, status, and date
 *              to the console. Handles cases where the payment is not found.
 * Parameters:
 *    paymentId - A string representing the unique identifier of the payment to be displayed.
 * Returns: None
 */
void UserInterface::displayPaymentStatus(const std::string& paymentId)
{
	std::string bookingId;
	double amount = 0.0;
	Enums::PaymentMethod paymentMethod;
	Enums::PaymentStatus paymentStatus;
	std::string paymentDate;
	if (m_controller->viewPaymentStatus(paymentId, bookingId, amount, paymentMethod, paymentStatus, paymentDate) == Enums::ProcessStatus::FAILED)
	{
		std::cout << "\nPayment not found!";
		return;
	}
	std::cout << "\nPayment ID   : " << paymentId;
	std::cout << "\nBooking ID   : " << bookingId;
	std::cout << "\nAmount       : " << amount;
	std::cout << "\nMethod       : " << static_cast<int>(paymentMethod);
	std::cout << "\nStatus       : " << static_cast<int>(paymentStatus);
	std::cout << "\nPayment Date : " << paymentDate;
}

/*
 * Function: UserInterface::viewPaymentStatus
 * Description: Prompts the user to enter a payment ID and then calls
 *              displayPaymentStatus to show the corresponding payment details.
 * Parameters: None
 * Returns: None
 */
void UserInterface::viewPaymentStatus()
{
	std::string paymentId;
	cout << "\nEnter payment Id: ";
	util::readValue(paymentId);
	displayPaymentStatus(paymentId);
}

/*
Function Name : selectPaymentMethod
Description   : Allows the user to select a payment method, validates
				the input, and initiates payment through the controller.
				Generates a ticket upon successful payment.
Parameters    :
				 bookingId - The unique identifier of the booking
				 amount    - The payment amount
Return Type   : void
*/
void UserInterface::selectPaymentMethod(const std::string& bookingId, double amount)
{
	int choice = displayPaymentOptions();
	Enums::PaymentMethod type = Enums::PaymentMethod::CREDIT_CARD;
	Enums::ProcessStatus valid;
	if (choice == 1)
	{
		type = Enums::PaymentMethod::CREDIT_CARD;
		valid = handleCardPayment(type);
	}
	else if (choice == 2)
	{
		type = Enums::PaymentMethod::DEBIT_CARD;
		valid = handleCardPayment(type);
	}
	else if (choice == 3)
	{
		valid = handleUPIPayment(type);
	}
	else
	{
		std::cout << "Error: Invalid payment method.\n";
		return;
	}
	std::cout << "Payment method selected successfully.\n";
	Enums::ProcessStatus result = Enums::ProcessStatus::FAILED;
	//result = m_controller->initiatePayment(bookingId, type, amount);
	if (result == Enums::ProcessStatus::SUCCESS)
	{
		std::cout << "Payment completed and ticket generated.\n";
	}
	else
	{
		std::cout << "Payment failed.\n";
	}
}
