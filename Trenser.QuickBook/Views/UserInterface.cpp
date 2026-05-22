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
		cout << "1. Add Movie" << endl;
		cout << "2. Updated Movie Details" << endl;
		cout << "0. Exit "<< endl;
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
		case 0:
			isMenuActive = false;
			break;
		case 1:
			addMovie();
			break;
		case 2:
			
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
	string title, movieId, genre, language;
	int choice = 1, duration;
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
			displayCurrentMovieDetails(movieId, movies);
			const Movie* currentMovie = getCurrentMovie(movieId, movies);
			while (choice != 0)
			{
				displayEditMovieMenu();
				util::readValue(choice);
				if (choice == 1)
				{
					cout << "\nEnter the new Title: ";
					util::readValue(title);
					changeMovieTitle(movieId, title, currentMovie);
				}
				else if (choice == 2)
				{
					cout << "\nEnter the new Language: ";
					util::readValue(language);
					changeMovieLanguage(movieId, language, currentMovie);
				}
				else if (choice == 3)
				{
					cout << "\nEnter the new Genre: ";
					util::readValue(genre);
					changeMovieGenre(movieId, genre, currentMovie);
				}
				else if (choice == 4)
				{
					cout << "\nEnter the new Duration: ";
					util::readValue(duration);
					changeMovieDuration(movieId, duration, currentMovie);
				}
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
	cout << left << setw(30) << "ID"
		<< setw(10) << "Title"
		<< setw(10) << "Language"
		<< setw(10) << "Genre"
		<< setw(10) << "Duration" << endl;
	cout << "-------------------------------------------------------------\n";
	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		const Movie* movie = *iterator;
		if (movie)
		{
			cout << left << setw(10) << movie->getMovieId()
				<< setw(20) << movie->getTitle()
				<< setw(15) << movie->getLanguage()
				<< setw(15) << movie->getGenre()
				<< setw(10) << movie->getDuration()
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
const std::vector<std::string>& UserInterface::getMovieIdFromList(const std::vector<const Movie*>& movies)
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
	cout << left << setw(30) << "ID"
		<< setw(10) << "Title"
		<< setw(10) << "Language"
		<< setw(10) << "Genre"
		<< setw(10) << "Duration" << endl;
	cout << "-------------------------------------------------------------\n";
	for (std::vector<const Movie*>::const_iterator iterator = movies.begin(); iterator != movies.end(); ++iterator)
	{
		const Movie* movie = *iterator;
		if (movie->getMovieId() == movieId)
		{
			cout << left << setw(10) << movie->getMovieId()
				<< setw(20) << movie->getTitle()
				<< setw(15) << movie->getLanguage()
				<< setw(15) << movie->getGenre()
				<< setw(10) << movie->getDuration()
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
		const Movie* movie = *iterator;
		if (movie->getMovieId() == movieId)
		{
			return movie;
		}
	}
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
void UserInterface::changeMovieTitle(string& movieId, string& title, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(title, currentMovie->getLanguage(), currentMovie->getGenre(), currentMovie->getDuration()))
	{
		if (m_controller->setMovieTitle(movieId, title) == Enums::ProcessStatus::SUCCESS)
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
void UserInterface::changeMovieLanguage(string& movieId, string& language, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), language, currentMovie->getGenre(), currentMovie->getDuration()))
	{
		if ((m_controller->setMovieLanguage(movieId, language) == Enums::ProcessStatus::SUCCESS))
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
void UserInterface::changeMovieGenre(string& movieId, string& genre, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), currentMovie->getLanguage(), genre, currentMovie->getDuration()))
	{
		if ((m_controller->setMovieGenre(movieId, genre) == Enums::ProcessStatus::SUCCESS))
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
void UserInterface::changeMovieDuration(string& movieId, int& duration, const Movie*& currentMovie)
{
	if (Enums::ProcessStatus::SUCCESS == handleMovieDetailsInput(currentMovie->getTitle(), currentMovie->getLanguage(), currentMovie->getGenre(), duration))
	{
		if ((m_controller->setMovieDuration(movieId, duration) == Enums::ProcessStatus::SUCCESS))
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