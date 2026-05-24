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
using namespace std;
#include "InputHelper.h"
#include "OutputHelper.h"
#include "Enums.h"
#include "Validator.h"
#include "UserInterface.h"
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
	cout << "1. Add Theatre" << endl;
	cout << "2. View Theatre Details" << endl;
	cout << "3. Update Theatre Details" << endl;
	cout << "0. Exit" << endl;
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
		case 0:
			isMenuActive = false;
			break;
		case 1:
			addTheatre();
			break;
		case 2:
			viewTheatreDetails();
			break;
		case 3:
			updateTheatre();
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
	int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
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
 * Function: UserInterface::getScreenId
 * Description: Displays available screens and validates the selected screen ID.
 * Parameters:
 *    screens (const std::vector<const Screen*>&) - List of available screens
 *    screenId (std::string&) - Reference to store selected screen ID
 * Returns:
 *    bool - True if a valid screen ID is selected, false otherwise
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
		if ((*iterator)->getTheatreId()==theatreId)
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
					changeTheatreName(theatreId, name,theatre);
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
					changeTheatreEmail(theatreId, email,theatre);
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
		cout << "No theatres found for current owner" << endl;
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


