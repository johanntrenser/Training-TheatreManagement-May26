/*
 * File: UserInterface.cpp
 * Description: Implements the UserInterface class responsible for handling
 *              user interactions with the Quick Book - Theatre Management System.
 * Author: Trenser
 * Created: 20 May 2026
 */
#include <stdexcept>
#include <string>
#include <iomanip>
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
		cout << "1. Create User\n2. View All Users\n3. Update User Details\n4. Deactivate User\n5. Reactivate User\n6. View Profile\n7. View User Status\n8. Change Password\n9. Logout\n10. Add Movie\n11. Update Movie Details\n12. List All Movies\n13. Deactivate Movie\n14. Reactivate Movie\n15. Search Movie" << endl;
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
	cout << "1. Update User Details\n2. View Profile\n3. Change Password\n4. Logout" << endl;
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
	cout << "1. Update User Details\n2. View Profile\n3. Change Password\n4. Logout" << endl;
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
 * Description: Handles the admin menu loop, displaying options and processing user input.
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
			isMenuActive = false;
			break;
		case 10:
			addMovie();
			break;
		case 11:
			updateMovie();
			break;
		case 12:
			displayAllMovies();
			break;
		case 13:
			DeactivateMovie();
			break;
		case 14:
			activateMovie();
			break;
		case 15:
			searchMovie();
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
 * Description: Handles the customer menu loop, displaying options and processing user input.
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
 * Description: Handles the theatre owner menu loop, displaying options and processing user input.
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
Enums::ProcessStatus UserInterface::checkMovieIdIsValid(string movieId, const std::vector<string> movieIdList)
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
void UserInterface::displayCurrentMovieDetails(string& movieId, const std::vector<const Movie*>& movies)
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
const Movie* UserInterface::getCurrentMovie(string& movieId, const std::vector<const Movie*>& movies)
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
	std::vector<const Movie*> movies = m_controller->getAllActiveMovies();
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
	string title, movieId;
	cout << "\nEnter the movie title: ";
	util::readValue(title);
	const std::vector<const Movie*> movies = m_controller->searchMovieByTitle(title);
	if (!movies.empty())
	{
		displayMovie(movies);
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
